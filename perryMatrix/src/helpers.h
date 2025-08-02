// © 2025 SC5K Systems

#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <stdlib.h>
#include <math.h>
#include "matrix_config.h"

/*
  getRandomChar
  return a random printable ascii character (codes 33–126)
  used for obfuscation and scrambling text effects
*/
inline char getRandomChar() {
  return (char)random(33, 127);
}

/*
  color565
  convenience wrapper around matrix.color565(r,g,b)
  converts 8-bit r,g,b values to 16-bit rgb565 format
*/
inline uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return matrix.color565(r, g, b);
}

/*
  shuffleArray
  perform in-place Fisher–Yates shuffle on a uint16_t array of length n
  used in color-test, dynamic network, and loader obfuscation routines
*/
inline void shuffleArray(uint16_t *arr, uint16_t n) {
  for (uint16_t i = n - 1; i > 0; i--) {
    uint16_t j = random(0, i + 1);
    uint16_t t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
  }
}

/*
  hsvToRgb
  convert hue (0–360°), saturation (0–1), value (0–1)
  into 0–255 r,g,b components
  for smooth color transitions based on HSV space
*/
inline void hsvToRgb(float h, float s, float v,
                     uint8_t &r, uint8_t &g, uint8_t &b) {
  int   i = int(h / 60.0f) % 6;
  float f = (h / 60.0f) - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);
  float rf, gf, bf;
  switch (i) {
    case 0: rf = v;  gf = t;  bf = p;  break;
    case 1: rf = q;  gf = v;  bf = p;  break;
    case 2: rf = p;  gf = v;  bf = t;  break;
    case 3: rf = p;  gf = q;  bf = v;  break;
    case 4: rf = t;  gf = p;  bf = v;  break;
    default:rf = v;  gf = p;  bf = q;  break;
  }
  r = uint8_t(rf * 255);
  g = uint8_t(gf * 255);
  b = uint8_t(bf * 255);
}
