# Keyball macOS Scroll Bridge

Experimental user-space bridge for smooth Keyball scrolling on macOS. The
Keyball firmware sends high-resolution wheel deltas over QMK Raw HID, and this
helper posts them as continuous pixel scroll events through CoreGraphics.

The helper does not emulate an Apple trackpad. This first version provides
precise pixel scrolling without momentum or gestures; momentum can be added
after the raw motion has been tested and tuned on a Mac.

## Requirements

- macOS 13 or later
- Xcode 15 Command Line Tools (Swift 5.9) or later
- The `keyball/keyball39:jjs` firmware from this repository
- Accessibility permission for the built executable

## Build

Build and flash the firmware from the main QMK checkout:

```sh
cd ~/tools/qmk_firmware
nix-shell --run 'make keyball/keyball39:jjs'
```

Build the helper on macOS:

```sh
cd ~/tools/keyball/macos/keyball-scroll
swift build -c release
```

## Run

```sh
.build/release/keyball-scroll --verbose
```

macOS will request Accessibility access. Grant access under **System Settings >
Privacy & Security > Accessibility**, then restart the helper.

Useful tuning options:

```text
--pixels-per-tick N   Pixel distance for one conventional wheel tick (default: 40)
--invert-vertical     Reverse vertical scrolling
--invert-horizontal   Reverse horizontal scrolling
--verbose             Print connection and packet diagnostics
```

The firmware requires a heartbeat from the helper every 500 ms. If the helper
stops or the Raw HID interface disconnects, the firmware returns to ordinary
wheel reports after 1.5 seconds.

## Protocol

Both directions use fixed 32-byte Raw HID reports on usage page `0xFF60`, usage
`0x61`. The shared protocol definition is
`qmk_firmware/keyboards/keyball/lib/keyball/scroll_bridge_protocol.h`.

| Offset | Size | Field |
| ------ | ---- | ----- |
| 0 | 3 | Magic `KBS` |
| 3 | 1 | Protocol version |
| 4 | 1 | Message type |
| 5 | 1 | Sequence number |
| 6 | 1 | Flags |
| 8 | 2 | Signed vertical delta, little-endian |
| 10 | 2 | Signed horizontal delta, little-endian |
| 12 | 2 | Wheel-tick resolution, little-endian |

Message types are `0x01` host probe, `0x02` host goodbye, `0x03` host
heartbeat, `0x81` device hello, and `0x82` device scroll. A probe does not
disable ordinary wheel reports; only acknowledged heartbeats activate the raw
stream.
