// © 2025 SC5K Systems

#pragma once
#include <Arduino.h>
#include "matrix_config.h"  // matrix instance, WIDTH, HEIGHT
#include "globals.h"        // FFT object, samples, barHeights[], etc.

/*/
readFFT
sample mic pin at fixed rate into vReal[], apply window & FFT, compute magnitudes,
map bins 2…(WIDTH/3+1) into barHeights[] and redlined[] flags
/*/
void readFFT();

/*/
updatePeaks
for each bar, if current height > peak, reset peak+timestamp; otherwise,
decay peak over time after a brief hold
/*/
void updatePeaks();

/*/
getBarColor
given ypos and bar height, return a gradient color from blue→magenta→green
/*/
uint16_t getBarColor(int y, int h);

/*/
drawWaveform
render raw audio waveform across rotated display by plotting each sample pixel
/*/
void drawWaveform();

/*/
drawBars
render vertical bars for each FFT bin plus peak markers; use getBarColor and redlined[]
/*/
void drawBars();

/*/
initAudioVis
prepare pins, clear buffers (barHeights, redlined, peakLevels, smoothedInput),
set audioActive & startTime, clear screen
/*/
void initAudioVis();

/*/
runAudioVisFrame
if not ready or duration expired, bail out; else
  - readFFT, updatePeaks
  - clear screen, drawWaveform, drawBars, show()
  - tiny delay for stability
/*/
void runAudioVisFrame();
