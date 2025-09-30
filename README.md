# perryMatrix

perryMatrix is an Arduino sketch for the Adafruit Matrix Portal M4 driving a 128×32 RGB Protomatter panel.  
It switches modes via USB/Serial1 commands sent as:

## core modes
- **mode 0 (checklist)** — animated match-setup checklist (`checklist.cpp`)
- **mode 1 (autonomous)** — AUTO LOCK starfield + circles animation (`autonomous.cpp`)
- **mode 2 (dynamic)** — moving node network with optional accel readout, AI flag and tiny 3D cube (`dynamic.cpp`)

## secondary features
- **shutdown** — “match over” header + scrolling fake code (`shutdown.cpp`)
- **sponsor scroller** — scrolling sponsor banners (`sponsor_scroller.cpp`)
- **perry loader** — text obfuscation / decrypt animation (`perry_loader.cpp`)
- **audio visualizer** — FFT bars + peaks from mic input (`audio_vis.cpp`)

## structure
- `matrix_config.*` — pinout, matrix init, constants
- `globals.*` — global state
- `helpers.h` — small utils (rgb565, hsv→rgb, shuffle, random chars)

## build
open `perryMatrix.ino` in Arduino IDE  
install: `Adafruit_GFX`, `Adafruit_Protomatter`, `Adafruit_LIS3DH`, `ArduinoFFT`  
select Matrix Portal M4 board, compile + upload

© 2025 SC5K Systems
