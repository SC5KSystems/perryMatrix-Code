// © 2025 SC5K Systems

#pragma once

#include <Arduino.h>
#include "matrix_config.h"
#include "globals.h"
#include "helpers.h"

/*/
drawStaticHeader
draw the “spon-” and “-sors” text at top and a dotted divider line
/*/
void drawStaticHeader();

/*/
wheel
map 0–255 to an rgb565 rainbow color (red→green→blue cycle)
/*/
uint16_t wheel(uint8_t pos);

/*/
initSponsorScroller
seed random, choose first sponsor, pick random x & hue, set y below screen
/*/
void initSponsorScroller();

/*/
runSponsorScroller
clear screen, draw current sponsor scrolling up, redraw header,
then loop to next sponsor once off-screen
/*/
void runSponsorScroller();
