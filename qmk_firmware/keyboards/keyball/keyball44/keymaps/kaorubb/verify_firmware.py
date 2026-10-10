"""Check the built AVR firmware against key positions transcribed from the Remap PDFs."""
import re, struct, subprocess, unittest
from pathlib import Path


class FirmwareTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        qmk = Path(__file__).resolve().parents[6]/'tmp/qmk-0.29.0'
        stem = 'keyball_keyball44_kaorubb'
        output = subprocess.check_output(['avr-nm', '-S', '--defined-only', str(qmk/'.build'/f'{stem}.elf')], text=True)
        cls.symbols = {name: (int(addr,16),int(sz,16)) for addr,sz,name in re.findall(r'^([0-9a-f]+) ([0-9a-f]+) \w (\S+)$', output, re.M)}
        cls.memory = {}
        for line in (qmk/f'{stem}.hex').read_text().splitlines():
            record = bytes.fromhex(line[1:])
            if sum(record)%256: raise ValueError('Invalid Intel HEX checksum')
            if record[3] == 0:
                addr = int.from_bytes(record[1:3], 'big')
                cls.memory.update(enumerate(record[4:-1], addr))
        addr,sz = cls.symbols['keymaps']
        data = bytes(cls.memory[i] for i in range(addr,addr+sz))
        cls.keys = [struct.unpack_from('<48H', data, i) for i in range(0,sz,96)]

    def row(self, layer, row): return self.keys[layer][row*6:row*6+6]

    def test_flash_capacity_and_layer_width(self):
        self.assertLess(max(self.memory), 28672)
        self.assertEqual(self.symbols['layer_state'][1], 2)
        self.assertEqual(self.symbols['default_layer_state'][1], 2)

    def test_right_ball_geometry(self):
        self.assertEqual(len(self.keys), 8)
        for layer in self.keys:
            for row,col in ((3,0), (7,0), (7,2), (7,3)):
                self.assertEqual(layer[row*6+col], 0)

    def test_base_typing_and_modifiers(self):
        for layer in (0,4):
            self.assertEqual(self.row(layer,0), (0x2b,0x14,0x1a,0x08,0x15,0x17))  # Tab Q W E R T
            self.assertEqual(self.row(layer,4), (0x2e,0x13,0x12,0x0c,0x18,0x1c))  # = P O I U Y
            self.assertEqual(self.row(layer,2), (0xe1,0x1d,0x1b,0x06,0x19,0x05))  # Shift Z X C V B
            self.assertEqual(self.row(layer,6), (0x322d,0x38,0x37,0x36,0x10,0x11))
        self.assertEqual(self.row(0,1), (0x2129,0x2104,0x2816,0x07,0x09,0x0a))
        self.assertEqual(self.row(4,1), (0x2129,0x2104,0x2416,0x07,0x09,0x0a))
        self.assertEqual(self.row(0,5), (0x3134,0x3133,0x380f,0x0e,0x0d,0x0b))
        self.assertEqual(self.row(4,5), (0x3134,0x3133,0x340f,0x0e,0x0d,0x0b))
        self.assertEqual(self.row(0,3)[1:], (0,0x43a8,0x52c1,0x242c,0x2a))
        self.assertEqual(self.row(4,3)[1:], (0x213c,0x47a8,0x52c5,0x2c,0x2a))
        self.assertEqual(self.row(0,7), (0,0x3128,0,0,0xe6,0xe5))
        self.assertEqual(self.row(4,7), (0,0x3128,0,0,0xe3,0xe5))

    def test_number_layer_differences_and_mouse_buttons(self):
        self.assertEqual(self.row(1,1), (0xe0,0x2123,0x2824,0x25,0x26,0x27))
        self.assertEqual(self.row(5,1), (0xe0,0x2123,0x2424,0x2225,0x2826,0x27))
        self.assertEqual(self.row(1,6), (0x322d,0x31,0x37,0x4b,0x4e,0x081b))
        self.assertEqual(self.row(5,6), (0x322d,0x31,0x37,0x36,0x10,0x11))
        # Basic mouse buttons are QMK 0.29.0 codes; shifted click is a chord, not Mod-Tap.
        for layer in (1,5):
            self.assertEqual(self.row(layer,5)[3:], (0xd2,0xd1,0x02d1))
            self.assertEqual(self.row(layer,2)[1], 0x7e07)  # momentary scroll
            self.assertEqual(self.row(layer,7)[1], 0x0333)  # Ctrl+Shift+semicolon

    def test_settings_and_profile_selection(self):
        for layer in (3,7):
            self.assertEqual(self.row(layer,3)[4:], (0x7e40,0x7e41))
            self.assertEqual(self.row(layer,5)[0], 0xa9)  # Volume up
            self.assertEqual(self.row(layer,4)[0:2], (0xaa,0xa8))  # Volume down, Mute
            self.assertEqual(self.row(layer,3)[1:3], (0x7e00,0x7e00))  # PDF's two Keyball reset keys
        self.assertEqual(self.row(3,4)[3], 0x45)  # Linux F12
        self.assertEqual(self.row(7,4)[3], 0x41)  # Mac F8

    def test_shortcut_chords(self):
        for layer in (2,6):
            self.assertEqual(self.row(layer,0), (0xe2,0x011e,0x011f,0x0120,0x0121,0x0122))
            self.assertEqual(self.row(layer,2), (1,0x051e,0x051f,0x0520,0x0521,0x0522))
            self.assertEqual(self.row(layer,4), (0x0515,0x0513,0x0512,0x41,0x40,0x3f))


if __name__ == '__main__': unittest.main()
