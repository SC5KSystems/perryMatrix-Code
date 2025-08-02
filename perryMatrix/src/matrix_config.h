// © 2025 SC5K Systems

#pragma once
#include <Arduino.h>           // pinMode, A1/A2, etc.
#include <Adafruit_GFX.h>
#include <Adafruit_Protomatter.h>

/*/
matrix dimensions
/*/
#define WIDTH   128
#define HEIGHT  32

/*/
character cell size (in pixels)
CHAR_W/CHAR_H for text layout
/*/
extern const uint8_t charW;
extern const uint8_t charH;
#define CHAR_W charW
#define CHAR_H charH

/*/
audio-visualizer pins & constants
MIC_PIN for analog mic input, GAIN_PIN to enable amplifier
MAX_BAR_HEIGHT for FFT bars
/*/
#define MIC_PIN         A1
#define GAIN_PIN        A2
#define MAX_BAR_HEIGHT  16

/*/
rgb matrix pin mappings
rgbPins[6] for R,G,B lines, addrPins[5] for row address lines,
clockPin, latchPin, oePin control scan timing
/*/
extern uint8_t rgbPins[];
extern uint8_t addrPins[];
extern uint8_t clockPin, latchPin, oePin;

/*/
shared serial buffer pointer, allocated in .cpp
/*/
extern char *buf;

/*/
primary matrix object
constructed with WIDTH×HEIGHT, dual buffer enabled
/*/
extern Adafruit_Protomatter matrix;
