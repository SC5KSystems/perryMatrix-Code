perryMatrix is a "small" Arduino sketch for a Matrix Portal M4 driving a 128×32 RGB Protomatter panel. 
It implements three modes that you switch between by sending a line over USB or Serial1 in the form:

<mode> [payload]\n

• mode 0 (“checklist”) animates a match-setup checklist 
(see checklist.cpp for processChecklistPayload, drawChecklistStatic and related code).
• mode 1 (“autonomous”) runs the AUTO LOCK starfield and circles animation 
(see autonomous.cpp and runAutonomousFrame).
• mode 2 (“dynamic”) shows a moving node network plus optional accelerometer readout, "AI" status 
indicator and tiny 3D cube—flags are parsed by updateDynamicFromPayload and frames drawn by 
runDynamicFrame in dynamic.cpp.

beyond those core modes there’s a sponsor scroller (sponsor_scroller.cpp), the “perry loader” text 
obfuscation/decryption effect (perry_loader.cpp), and a real-time audio visualizer using FFT 
(audio_vis.cpp). shared constants, pin mappings, matrix instance and global state live in matrix_config.* 
and globals.*, while tiny utilities like color conversion, shuffling and hsv→rgb live in helpers.h.

to build, open perryMatrix.ino in the arduino ide, install the adafruit_gfx, adafruit_protomatter, 
adafruit_lis3dh and arduinofft libraries, select your board and upload. all documentation lives 
alongside each module in its *.h file.

© 2025 SC5K Systems
