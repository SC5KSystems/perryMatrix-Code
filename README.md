# perryMatrix

perryMatrix is a custom Arduino sketch built for the Adafruit Matrix Portal M4 and a 128×32 RGB Protomatter panel.  
It listens over USB or Serial1 (RS-232), and a simple line of text is all it takes to flip the display into a new world.  

There are four core functions with the included cases: 

**Checklist/Init Setup Mode (0)**  
The field crew’s match setup displays the panel as an animated checklist. Each item fills in, boxes flip from red to green, and the bottom bar shows “READY” when all systems line up. When the robot is ready for 10 seconds, some mini-functions that are essentially individual programs integrated into one big program start cycling. The first is **Sponsor Scroller**, which brings our team's sponsor names across the display with clean vertical motion. The second is **Perry Loader**, which writes on obfuscated text, then decrypts it to show our robot's name, "Perry The Periscope" in real time. The third and final mini-function is **Audio Visualizer**, which slams the panel with horizontal reflected FFT bars, peak holds, and color dynamics that react to the sound around you. This is the only function that needs external hardware (a microphone) to be hooked up directly into the microcontroller. If anything were to change in the checklist (e.g. the robot loses connection to the Field Management Systems (FMS), the idle animations immediately clear and the checklist displays again.

This function is probably the most important and useful out of all of them.

**Autonomous Mode (1)**  
AUTO LOCK is taken straight out of the escape pod scene from WALL-E (please don't sue me Disney). It doesn't do much other than provide a pretty and dramatic animation during the autonomous period.

**Dynamic Mode (2)**  
Several sub-modes are implemented here. When the robot goes to the feeder to load a game piece, it will display FETCH PIECE in flashing red and green, and when a game piece is successfully loaded, a "top view" of that piece will slide frame in from the top of the matrix. When it scores, a reverse counter appears and after a second displays what level it is scoring on. An intensity meter is also the background of the scoring animation for more seizure-inducing LED raves. For climbing, two boxes similar to the checklist box will appear as the robot is lined up to the game element. When it is ready to climb, the boxes will flash green and red, and if the accelorometer on the robot detects a constant motion the matrix will display "WHAT A CLIMB" just to be funny.

**Shutdown Mode (3)**
Similar to the Autonomous mode in terms of functionality, this will display a red MATCH OVER on top of the matrix and fake shutdown code will display, kind of like hackertyper.net if you need me to elaborate. In fact, exactly like hackertyper.net. Allegedly.

Some of the important boring modules:  
`matrix_config.*` sets the pins, constants, and matrix instance.  
`globals.*` keeps the shared state.  
`helpers.h` stashes little utilities like rgb565 conversion, hsv→rgb, shuffles, and random character generation.  

To build: open `perryMatrix.ino` in Arduino IDE.  
Make sure `Adafruit_GFX`, `Adafruit_Protomatter`, `Adafruit_LIS3DH`, and `ArduinoFFT` are installed.  
Select the Matrix Portal M4 board, hit upload, and reap the benefits of plagarism.

© 2025 SC5K Systems
