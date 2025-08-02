// © 2025 SC5K Systems

#pragma once
#include "globals.h"
#include <Arduino.h>

/*/
resetStar
place star in center with random heading and speed for autonomous animation
/*/
void resetStar(Star &s);

/*/
initAutonomous
start autonomous mode: clear screen, reset timers, seed stars & circles,
and compute positions for the \"AUTO LOCK\" box
/*/
void initAutonomous();

/*/
runAutonomousFrame
render one frame of auto mode: toggle text, draw expanding circles,
move stars, and display the \"AUTO LOCK\" box
/*/
void runAutonomousFrame();
