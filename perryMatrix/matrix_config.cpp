// © 2025 SC5K Systems

#include "src/matrix_config.h"
#include <Arduino.h>

/*/
character dimensions
must match font metrics (6×8)
charW/charH used via CHAR_W/CHAR_H
/*/
const uint8_t charW = 6;
const uint8_t charH = 8;

/*/
matrix wiring
define rgb and address pin arrays, plus clock/latch/oe
/*/
uint8_t rgbPins[]  = {7,  8,  9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20, 21};
uint8_t clockPin   = 14;
uint8_t latchPin   = 15;
uint8_t oePin      = 16;

/*/
serial buffer pointer
initialize to nullptr; main .ino will allocate size into buf
/*/
char *buf = nullptr;

/*/
matrix instance
WIDTH×HEIGHT, 6 rgb pins, 1 chain, 5 addr pins,
clockPin, latchPin, oePin, double-buffer enabled
/*/
Adafruit_Protomatter matrix(
  WIDTH, 6, 1,
  rgbPins, 4, addrPins,
  clockPin, latchPin, oePin,
  true
);
