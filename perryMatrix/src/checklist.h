// © 2025 SC5K Systems

#pragma once
#include <Arduino.h>
#include <Stream.h>
#include <stdint.h>

/*/
drawChecklistStatic
render the full checklist: header (“MATCH SETUP”), each item text with
red/green box, and bottom “NOT READY” if any unchecked
/*/
void drawChecklistStatic();

/*/
processChecklistPayload
parse CSV payload of 0/1 flags, detect box changes, animate fill sweep
and flash bottom NOT/READY accordingly
/*/
void processChecklistPayload(const char* payload);

/*/
readAndProcess
read a line from given Stream into buf, log it, then call processChecklistPayload
/*/
void readAndProcess(Stream &in, const char *label);

/*/
sweepBoxLR
animate a box vertical fill left→right for index idx in color color
/*/
void sweepBoxLR(uint8_t idx, uint16_t color);

/*/
sweepBoxRL
animate a box vertical fill right→left for index idx in color color
/*/
void sweepBoxRL(uint8_t idx, uint16_t color);

/*/
drawDashedOutline
draw a dashed rectangle around (bx,by)-(bx+bw,by+bh) with perim dashes,
offset into pattern, using color
/*/
void drawDashedOutline(int bx, int by, int bw, int bh,
                       int perim, int offset, uint16_t color);