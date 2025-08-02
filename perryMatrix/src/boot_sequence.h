// © 2025 SC5K Systems

#pragma once
#include <Arduino.h>

/*/
handleRobotMessage
read from usb/robotio serial, parse mode and optional payload,
switch currentMode and call init/process routines accordingly
/*/
void handleRobotMessage();

/*/
initBootSequence
run one-time startup animations: splash text, type options, progressive outline,
blink “LED”, (color test block left intact), then initial checklist write-on
/*/
void initBootSequence();

/*/
runBootSequence
called in loop when in checklist mode: read/process checklist payload,
then sequence sponsor scroller → perry loader → audio vis based on state
/*/
void runBootSequence();
