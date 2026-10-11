#!/usr/bin/env python3
"""Back up, flash, or restore the USB-connected Keyball44 through Caterina."""
import argparse, hashlib, json, shutil, subprocess, sys, time
from datetime import datetime, timezone
from pathlib import Path

repo = Path(__file__).resolve().parents[1]
recovery = repo/'keyball44/doc/recovery'


def connected_usb():
    devices = []
    for p in Path('/sys/bus/usb/devices').iterdir():
        if (p/'idVendor').exists() and (p/'idVendor').read_text().strip() == '5957' and (p/'idProduct').read_text().strip() == '0400':
            devices.append(p.resolve())
    if len(devices) != 1: raise RuntimeError('Connect exactly one Keyball44 by USB before starting')
    return devices[0]


def wait_for_port(usb):
    print('Press the physical reset button twice now. Waiting up to 180 seconds...', flush=True)
    deadline = time.monotonic()+180
    while time.monotonic() < deadline:
        for p in Path('/sys/class/tty').glob('ttyACM*'):
            if usb in (p/'device').resolve().parents and Path('/dev',p.name).exists():
                print(f'Found /dev/{p.name}', flush=True)
                return '/dev/'+p.name
        time.sleep(0.1)
    raise TimeoutError('No Caterina port appeared on the connected Keyball44 USB path')


def application_hex(data):
    """Encode the application area from `data`, excluding Caterina's last 4 KiB."""
    records = []
    for addr in range(0,28672,16):
        record = bytes([16,addr >> 8,addr & 255,0])+data[addr:addr+16]
        records.append(':'+record.hex().upper()+f'{(-sum(record)) & 255:02X}')
    return '\n'.join(records)+'\n:00000001FF\n'


def finalize_backup(dest, side, usb_path):
    """Create restoration files from the successfully read AVR files in `dest`."""
    flash, eeprom = dest/'flash.bin', dest/'eeprom.bin'
    data = flash.read_bytes()
    # avrdude can omit trailing 0xFF bytes; the whole application area is what restoration needs.
    if not 28672 <= len(data) <= 32768 or len(eeprom.read_bytes()) != 1024:
        raise RuntimeError('Incomplete application or EEPROM backup; do not flash')
    if data[:28672] == b'\xff'*28672: raise RuntimeError('Application readback is empty; do not flash')
    app = dest/'application.hex'
    app.write_text(application_hex(data))
    manifest = {
        'side': side, 'captured_at_utc': dest.name.rsplit('-',1)[1], 'usb_path': usb_path,
        'files': {p.name: {'size': p.stat().st_size, 'sha256': hashlib.sha256(p.read_bytes()).hexdigest()} for p in (flash,eeprom,app)},
        'restore_note': 'Use application.hex for flash restoration; flash.bin includes bootloader bytes and may omit trailing 0xFF. Restore EEPROM with the original application.'
    }
    (dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(f'Backup completed: {dest}', flush=True)


def restoration_files(dest, side):
    """Verify the restoration files in `dest` belong to `side` and retain their original hashes."""
    manifest = json.loads((dest/'manifest.json').read_text())
    if manifest['side'] != side: raise ValueError('Backup side does not match the connected half')
    paths = [dest/'application.hex', dest/'eeprom.bin']
    for p in paths:
        expected = manifest['files'][p.name]
        data = p.read_bytes()
        if len(data) != expected['size'] or hashlib.sha256(data).hexdigest() != expected['sha256']:
            raise ValueError(f'Backup file has changed: {p}')
    return paths


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('operation', choices=['backup','flash','restore'])
    parser.add_argument('side', choices=['left','right'], help='Physical half connected by USB')
    parser.add_argument('--sudo', action='store_true', help='Authenticate in your terminal, then run only avrdude as root')
    parser.add_argument('--backup', type=Path, help='Completed backup directory to use for restore')
    args = parser.parse_args()
    avrdude = shutil.which('avrdude')
    if not avrdude: raise RuntimeError('Run inside the repository Nix development shell')
    firmware = repo/'tmp/qmk-0.29.0/keyball_keyball44_kaorubb.hex'
    if args.operation == 'restore':
        if args.backup is None: parser.error('restore requires --backup')
        app, eeprom = restoration_files(args.backup.resolve(), args.side)
    if args.operation == 'flash':
        subprocess.run([sys.executable, str(repo/'qmk_firmware/keyboards/keyball/keyball44/keymaps/kaorubb/verify_firmware.py')], check=True)
        print(f'Firmware SHA256: {hashlib.sha256(firmware.read_bytes()).hexdigest()}', flush=True)
    # Authenticate before the 8-second Caterina window starts. Never enter a password in chat.
    if args.sudo: subprocess.run(['sudo','-v'], check=True)
    usb = connected_usb()
    print(f'{args.operation} {args.side}: USB path {usb.name}', flush=True)
    port = wait_for_port(usb)
    command = (['sudo','-n'] if args.sudo else [])+[avrdude,'-p','atmega32u4','-c','avr109','-P',port]
    if args.operation == 'restore':
        subprocess.run(command+['-A','-U',f'flash:w:{app}:i','-U',f'eeprom:w:{eeprom}:r'], check=True)
        print(f'{args.side} original application and EEPROM restoration verified.', flush=True)
        return
    if args.operation == 'flash':
        subprocess.run(command+['-U',f'flash:w:{firmware}:i'], check=True)
        print(f'{args.side} flash write and verification completed.', flush=True)
        return
    stamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')
    dest = recovery/f'{args.side}-avr-backup-{stamp}'
    dest.mkdir()
    flash, eeprom = dest/'flash.bin', dest/'eeprom.bin'
    # Pre-create files as the invoking user so sudo does not make the backups root-owned.
    flash.touch()
    eeprom.touch()
    subprocess.run(command+['-A','-U',f'flash:r:{flash}:r','-U',f'eeprom:r:{eeprom}:r'], check=True)
    finalize_backup(dest, args.side, usb.name)


if __name__ == '__main__': main()
