// © 2025 SC5K Systems

#pragma once

#include <stdint.h>

/*/
initDynamic
set up the accelerometer, seed network nodes, reset display flags
/*/
void initDynamic();

/*/
updateDynamicFromPayload
parse up to three comma-separated flags [accel, ai, cube] (0 or 1 each), missing default to 0
/*/
void updateDynamicFromPayload(const char* payload);

/*/
runDynamicFrame
compute smoothed pitch/roll, read accel, clear screen, draw accel strip (if enabled), draw node network, draw ai blink and/or cube, then show frame
/*/
void runDynamicFrame();