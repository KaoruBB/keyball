"""Check restoration HEX encoding using the downloaded Keyball44 VIA firmware."""
import contextlib, io, json, runpy, tempfile, unittest
from pathlib import Path

avr = runpy.run_path(str(Path(__file__).with_name('keyball44-avr.py')))
encode = avr['application_hex']


class ApplicationHexTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        original = Path(__file__).resolve().parents[1]/'keyball44/doc/recovery/keyball44-via-v1.3.2.hex'
        cls.flash = bytearray(b'\xff'*32768)
        for line in original.read_text().splitlines():
            record = bytes.fromhex(line[1:])
            if record[3] == 0:
                addr = int.from_bytes(record[1:3], 'big')
                cls.flash[addr:addr+record[0]] = record[4:-1]
        cls.flash[28672:] = b'\x55'*4096
        cls.records = [bytes.fromhex(line[1:]) for line in encode(cls.flash).splitlines()]

    def test_bootloader_is_excluded(self):
        data = [r for r in self.records if r[3] == 0]
        self.assertEqual(len(data), 1792)
        self.assertEqual(max(int.from_bytes(r[1:3],'big')+r[0] for r in data), 28672)
        self.assertEqual(self.records[-1], bytes.fromhex('00000001ff'))

    def test_application_round_trip_and_checksums(self):
        restored = bytearray(28672)
        for record in self.records:
            self.assertEqual(sum(record)%256, 0)
            if record[3] == 0:
                addr = int.from_bytes(record[1:3], 'big')
                restored[addr:addr+record[0]] = record[4:-1]
        self.assertEqual(restored, self.flash[:28672])

    def test_completed_readback_with_trimmed_flash_tail(self):
        original = Path(__file__).resolve().parents[1]/'keyball44/doc/recovery/left-avr-backup-20261010T062915Z'
        with tempfile.TemporaryDirectory() as tmp:
            dest = Path(tmp)/original.name
            dest.mkdir()
            for name in ('flash.bin','eeprom.bin'): (dest/name).write_bytes((original/name).read_bytes())
            with contextlib.redirect_stdout(io.StringIO()): avr['finalize_backup'](dest,'left','1-7')
            self.assertEqual((dest/'application.hex').read_bytes(), (original/'application.hex').read_bytes())
            self.assertEqual(json.loads((dest/'manifest.json').read_text())['files']['flash.bin']['size'], 32762)

    def test_actual_restoration_files_match_manifest(self):
        original = Path(__file__).resolve().parents[1]/'keyball44/doc/recovery'
        for p in original.glob('*-avr-backup-*/manifest.json'):
            side = json.loads(p.read_text())['side']
            paths = avr['restoration_files'](p.parent,side)
            self.assertEqual([o.name for o in paths], ['application.hex','eeprom.bin'])
            with self.assertRaises(ValueError): avr['restoration_files'](p.parent, 'right' if side == 'left' else 'left')


if __name__ == '__main__': unittest.main()
