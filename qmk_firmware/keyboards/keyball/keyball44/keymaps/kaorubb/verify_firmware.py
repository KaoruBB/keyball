"""Check the built AVR keymap and Windows JIS translations."""
import json, re, struct, subprocess, unittest
from pathlib import Path


class FirmwareTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        qmk = Path(__file__).resolve().parents[6]/'tmp/qmk-0.29.0'
        stem = 'keyball_keyball44_kaorubb'
        output = subprocess.check_output(['avr-nm', '-S', '--defined-only', str(qmk/'.build'/f'{stem}.elf')], text=True)
        cls.symbols = {name: (int(addr,16),int(sz,16)) for addr,sz,name in re.findall(r'^([0-9a-f]+) ([0-9a-f]+) \w (\S+)$', output, re.M)}
        sections = subprocess.check_output(['avr-objdump', '-h', str(qmk/'.build'/f'{stem}.elf')], text=True)
        match = re.search(r'\s\d+ \.data\s+[0-9a-f]+\s+([0-9a-f]+)\s+([0-9a-f]+)', sections)
        cls.data_vma,cls.data_lma = (int(o,16) for o in match.groups())
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
        self.assertEqual(len(self.keys), 13)
        for layer in self.keys:
            for row,col in ((3,0), (7,0), (7,2), (7,3)):
                self.assertEqual(layer[row*6+col], 0)

    def test_common_mouse_layer(self):
        unused = {18,42,44,45}
        actions = {38: 0x7e07, 40: 0xd1, 39: 0xd2}  # Dot scroll, M left click, comma right click
        for pos,key in enumerate(self.keys[12]):
            with self.subTest(pos=pos):
                self.assertEqual(key, 0 if pos in unused else actions.get(pos,1))

    def test_compiled_jis_overrides(self):
        addr,sz = self.symbols['jis_overrides']
        addr += self.data_lma-self.data_vma
        data = bytes(self.memory[i] for i in range(addr,addr+sz))
        rows = list(struct.iter_unpack('<HBHBBHBHHH', data))
        expected = [
            (0x35,0,0x022f), (0x35,0x22,0x022e), (0x1f,0x22,0x2f),
            (0x23,0x22,0x2e), (0x24,0x22,0x0223), (0x25,0x22,0x0234),
            (0x26,0x22,0x0225), (0x27,0x22,0x0226), (0x2d,0x22,0x0287),
            (0x2e,0,0x022d), (0x2e,0x22,0x0233), (0x2f,0,0x30),
            (0x2f,0x22,0x0230), (0x30,0,0x32), (0x30,0x22,0x0232),
            (0x33,0x22,0x34), (0x34,0,0x0224), (0x34,0x22,0x021f),
            (0x31,0,0x87), (0x31,0x22,0x0289),
        ]
        self.assertEqual([(r[0],r[1],r[5]) for r in rows], expected)
        for row in rows:
            self.assertEqual(row[2], 0xffff)
            self.assertEqual(row[3], 0 if row[1] else 0x22)
            self.assertEqual(row[4], 0x22)
            self.assertEqual(row[-1], self.symbols['windows_profile'][0]-0x800000)

    def test_windows_layers_and_ime(self):
        changed = {(0,20): 0x4b7f, (0,21): 0x4991, (0,46): 0x3490,
                   (1,24): 0x89, (1,46): 1, (1,43): 0x0134, (2,37): 0x91}
        for layer in range(4):
            for pos,key in enumerate(self.keys[layer]):
                with self.subTest(layer=layer, pos=pos):
                    self.assertEqual(self.keys[layer+8][pos], changed.get((layer,pos),key))

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
        self.assertEqual(self.row(0,3)[1:], (0,0x437f,0x52c1,0x242c,0x2a))
        self.assertEqual(self.row(4,3)[1:], (0x213c,0x477f,0x52c5,0x2c,0x2a))
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
        for layer in (3,7,11):
            self.assertEqual(self.row(layer,3)[4:], (0x7e40,0x7e41))
            self.assertEqual(self.row(layer,7)[5], 0x7e42)
            self.assertEqual(self.row(layer,5)[0], 0x80)  # Keyboard Volume up
            self.assertEqual(self.row(layer,4)[0:2], (0x81,0x7f))  # Keyboard Volume down, Mute
            self.assertEqual(self.row(layer,3)[1:3], (0x7e00,0x7e00))  # PDF's two Keyball reset keys
        self.assertEqual(self.row(3,4)[3], 0x45)  # Linux F12
        self.assertEqual(self.row(7,4)[3], 0x41)  # Mac F8

    def test_shortcut_chords(self):
        for layer in (2,6):
            self.assertEqual(self.row(layer,0), (0xe2,0x011e,0x011f,0x0120,0x0121,0x0122))
            self.assertEqual(self.row(layer,2), (1,0x051e,0x051f,0x0520,0x0521,0x0522))
            self.assertEqual(self.row(layer,4), (0x0515,0x0513,0x0512,0x41,0x40,0x3f))

    def test_linux_matches_live_via_backup(self):
        backup = Path(__file__).resolve().parents[6]/'keyball44/doc/recovery/right-linux-via-readback.json'
        layers = json.loads(backup.read_text())['layers']
        # Missing switches under the ball and new profile selectors have no old key to reproduce.
        unused = {18,42,44,45}
        changed = {(0,19): 0, (2,46): 0xe5, (3,22): 0x7e40, (3,23): 0x7e41, (3,47): 0x7e42}
        for layer in range(4):
            for pos,old in enumerate(layers[layer]):
                if pos in unused: continue
                with self.subTest(layer=layer, row=pos//6, col=pos%6):
                    self.assertEqual(self.keys[layer][pos], changed.get((layer,pos),old))

    def test_mac_matches_live_via_backup_with_documented_changes(self):
        backup = Path(__file__).resolve().parents[6]/'keyball44/doc/recovery/left-mac-via-readback.json'
        layers = json.loads(backup.read_text())['layers']
        unused = {18,42,44,45}
        changed = {
            (0,20): 0x477f, (0,21): 0x52c5,  # local layers 3/1 become 7/5
            (2,46): 0xe3,  # 0x10e3 has a right-side marker with no mods; it sends Left GUI
            (3,7): 0x2144,  # user prefers PDF's Ctrl/F11 over the live plain F11
            (3,22): 0x7e40, (3,23): 0x7e41, (3,47): 0x7e42,
        }
        for layer in range(4):
            for pos,old in enumerate(layers[layer]):
                if pos in unused: continue
                with self.subTest(layer=layer, row=pos//6, col=pos%6):
                    self.assertEqual(self.keys[layer+4][pos], changed.get((layer,pos),old))


if __name__ == '__main__': unittest.main()
