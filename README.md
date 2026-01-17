[日本語版はこちら](./README.ja.md)

[![ubuntu](https://github.com/renatus-novus-x/joyout/workflows/ubuntu/badge.svg)](https://github.com/renatus-novus-x/joyout/actions?query=workflow%3Aubuntu)

<img src="https://raw.githubusercontent.com/renatus-novus-x/joyout/main/images/tether.gif" title="tether" />

# joyout
Atari 9-pin joyport output tester for X68000: maps keys 1–4 to four digital output lines (P1 pins 6/7/8 + P2 pin 8) for driving USB joystick adapters.

## Usage

1. Build `joyout.x` (see the Makefile in `src/`).
2. Run `joyout.x` on an X68000 (or a compatible environment).
3. Press keys `1`, `2`, `3`, `4` to toggle the four output lines **while held**:
   - `1` -> JoyPort1 pin 6
   - `2` -> JoyPort1 pin 7
   - `3` -> JoyPort1 pin 8
   - `4` -> JoyPort2 pin 8
4. Press `ESC` to quit.

### Notes
- This tool drives the Atari 9-pin joyport output lines via the system I/O (8255 Port C).
- Use a common GND when connecting to external devices (e.g., USB joystick adapters).

## Download
- [joyout.x](https://raw.githubusercontent.com/renatus-novus-x/joyout/main/bin/joyout.x)
