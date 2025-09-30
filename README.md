# perryMatrix

perryMatrix is a custom Arduino sketch built for the Adafruit Matrix Portal M4 and a 128×32 RGB Protomatter panel.  
It listens over USB or Serial1, and a simple line of text is all it takes to flip the display into a new world.  

At its heart, there are three main worlds you can jump between:

**Checklist Mode (0)**  
The field crew’s match setup runs across the panel as an animated checklist. Each item fills in, boxes flip from red to green, and the bottom bar shows “READY” when all systems line up. It’s a ritual every match deserves.

**Autonomous Mode (1)**  
AUTO LOCK takes over the matrix with drifting stars and orbiting circles. The screen becomes a simulation of the autonomous period — chaotic and sharp, yet calculated — with a highlight box anchoring the visuals.

**Dynamic Mode (2)**  
Here the panel turns into a living network. Nodes shift and connect, a 3D cube spins in wireframe, and extra flags bring up accelerometer readouts or AI status tags. It feels alive, like the machine is thinking.

That’s the core. But perryMatrix carries more tricks:

When the match ends, **Shutdown Mode** paints a “MATCH OVER” header and rolls out a hacker-style stream of fake code, line after line, scrolling up from the bottom.
When it’s time for sponsors to shine, **Sponsor Scroller** brings their names across the display with clean motion.  
For fun, the **Perry Loader** scrambles text into nonsense, then decrypts it back in real time.  
And if you hook up a mic, the **Audio Visualizer** slams the panel with FFT bars, peak holds, and color dynamics that react to the sound around you.

Under the hood it’s broken into modules:  
`matrix_config.*` sets the pins, constants, and matrix instance.  
`globals.*` keeps the shared state.  
`helpers.h` stashes little utilities like rgb565 conversion, hsv→rgb, shuffles, and random character generation.  

To build: open `perryMatrix.ino` in Arduino IDE.  
Make sure `Adafruit_GFX`, `Adafruit_Protomatter`, `Adafruit_LIS3DH`, and `ArduinoFFT` are installed.  
Select the Matrix Portal M4 board, hit upload, and watch the panel come alive.

© 2025 SC5K Systems
