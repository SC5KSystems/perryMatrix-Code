// © 2025 SC5K Systems

#include "src/dynamic.h"
#include "src/matrix_config.h" 
#include "src/globals.h"        
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS3DH.h>
#include <Fonts/TomThumb.h>
#include <math.h>

/*/
initNet
randomize node positions/directions within middle segment
/*/
static void initNet() {
  int16_t x_min = 0;
  int16_t x_max = matrix.width();
  int16_t y_min = SEG_TOP_H;
  int16_t y_max = SEG_TOP_H + SEG_MID_H;
  for (uint8_t i = 0; i < NODE_COUNT; i++) {
    nodes[i].x  = random(x_min, x_max);
    nodes[i].y  = random(y_min, y_max);
    nodes[i].dx = random(-1, 2);
    nodes[i].dy = random(-1, 2);
  }
}

void initDynamic() {
  Wire.begin();
  if (!lis.begin(0x18) && !lis.begin(0x19)) {
    Serial.println(F("LIS3DH not found at 0x18 or 0x19"));
  } else {
    lis.setRange(LIS3DH_RANGE_4_G);
  }
  initNet();
  filtRoll = 0.0f;
  filtPitch = 0.0f;
  accX = accY = accZ = 0.0f;
  showAccel = false;
  showAI = false;
  showCube = false;
}

/*/
updateDynamicFromPayload
parse up to three comma-separated flags [accel, ai, cube] (0 or 1 each), missing default to 0
/*/
void updateDynamicFromPayload(const char* payload) {
  int flags[3] = {0, 0, 0};
  if (payload && *payload) {
    char tmp[16];
    strncpy(tmp, payload, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    char *p = tmp;
    for (uint8_t i = 0; i < 3; i++) {
      flags[i] = atoi(p);
      char *comma = strchr(p, ',');
      if (!comma)
        break;
      p = comma + 1;
    }
  }
  showAccel = (flags[0] != 0);
  showAI    = (flags[1] != 0);
  showCube  = (flags[2] != 0);
}

/*/
drawNet
draw segment dividers and fill in node circles every frame, reseed every 4 frames
/*/
static void drawNet() {
  uint16_t border = matrix.color565(0, 255, 0);
  matrix.drawLine(0, SEG_TOP_H, matrix.width() - 1, SEG_TOP_H, border);
  matrix.drawLine(0, SEG_TOP_H + SEG_MID_H, matrix.width() - 1,
                  SEG_TOP_H + SEG_MID_H, border);
  if (++netFrame >= 4) {
    netFrame = 0;
    int16_t xmin = (matrix.width() - SEG_MID_H) / 2 + 2;
    int16_t xmax = xmin + SEG_MID_H - 4;
    for (uint8_t i = 0; i < NODE_COUNT; i++) {
      nodes[i].x += nodes[i].dx;
      nodes[i].y += nodes[i].dy;
      if (nodes[i].x < xmin || nodes[i].x > xmax)
        nodes[i].dx *= -1;
      if (nodes[i].y < SEG_TOP_H + 2 ||
          nodes[i].y > SEG_TOP_H + SEG_MID_H - 2)
        nodes[i].dy *= -1;
    }
  }
  uint16_t lineColor = matrix.color565(255, 255, 255);
  int drawn = 0;
  for (int i = 0; i < NODE_COUNT && drawn < 16; i++) {
    for (int j = i + 1; j < NODE_COUNT && drawn < 16; j++) {
      matrix.drawLine(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y,
                      lineColor);
      drawn++;
    }
  }
  for (uint8_t i = 0; i < NODE_COUNT; i++) {
    matrix.fillCircle(nodes[i].x, nodes[i].y, 1,
                      matrix.color565(0, 150, 255));
  }
}

/*/
rotProj
rotate by roll and pitch, project 3D point to 2D coords
/*/
static void rotProj(const float in[3], float pitch, float roll,
                    int16_t &sx, int16_t &sy,
                    int16_t cx, int16_t cy) {
  float x = in[0] * CUBE_SIZE;
  float y = in[1] * CUBE_SIZE;
  float z = in[2] * CUBE_SIZE;
  float cX = cosf(roll), sX = sinf(roll);
  float y1 = y * cX - z * sX;
  float z1 = y * sX + z * cX;
  float cY = cosf(pitch), sY = sinf(pitch);
  float x2 = x * cY + z1 * sY;
  float z2 = -x * sY + z1 * cY;
  float zcam = CAMERA_DIST + z2;
  float xp   = (x2 * FOV) / zcam;
  float yp   = (y1 * FOV) / zcam;
  sx = int16_t(xp + cx);
  sy = int16_t(cy - yp);
}

/*/
drawTinyCube
draw cube edges and colored axes at given origin
/*/
static void drawTinyCube(int16_t x0, int16_t y0) {
  int16_t cx = x0 + 12;
  int16_t cy = y0 + 24;
  int16_t vx[8], vy[8];
  for (uint8_t i = 0; i < 8; i++) {
    rotProj(VERTS[i], filtPitch, filtRoll, vx[i], vy[i], cx, cy);
  }
  uint16_t white = matrix.color565(255, 255, 255);
  for (uint8_t i = 0; i < 12; i++) {
    matrix.drawLine(vx[EDGES[i][0]], vy[EDGES[i][0]], vx[EDGES[i][1]],
                    vy[EDGES[i][1]], white);
  }
  const uint16_t cols[3] = {
      matrix.color565(255, 0, 0), matrix.color565(0, 255, 0),
      matrix.color565(0, 0, 255)};
  int16_t axp, ayp;
  for (uint8_t a = 0; a < 3; a++) {
    rotProj(AXIS_V[a], filtPitch, filtRoll, axp, ayp, cx, cy);
    matrix.drawLine(cx, cy, axp, ayp, cols[a]);
  }
}

/*/
runDynamicFrame
(read accel, filter angles, clear, draw optional accel strip, network, ai blink, cube, show)
/*/
void runDynamicFrame() {
  sensors_event_t evt;
  lis.getEvent(&evt);
  accX = evt.acceleration.y / 9.81f;
  accY = evt.acceleration.z / 9.81f;
  accZ = evt.acceleration.x / 9.81f;
  float pitchRaw = atan2f(-accX, sqrtf(accY * accY + accZ * accZ));
  float rollCand = atan2f(accY, accZ);
  float delta = rollCand - filtRoll;
  delta = fmodf(delta + M_PI, 2 * M_PI);
  if (delta < 0)
    delta += 2 * M_PI;
  delta -= M_PI;
  float rollRaw = (fabsf(delta) < ANGLE_THRESHOLD) ? rollCand : filtRoll;
  filtRoll = filtRoll * (1.0f - SMOOTHING) + rollRaw * SMOOTHING;
  filtPitch = filtPitch * (1.0f - SMOOTHING) + pitchRaw * SMOOTHING;
  matrix.fillScreen(0);
  if (showAccel) {
    matrix.setFont(&TomThumb);
    matrix.setTextSize(1);
    for (uint8_t k = 0; k < 3; k++) {
      int16_t y = k * 12;
      matrix.setTextColor(matrix.color565(255, 255, 255));
      matrix.setCursor(0, y);
      if (k == 0) {
        matrix.print("X:");
        matrix.print(accX, 2);
      } else if (k == 1) {
        matrix.print("Y:");
        matrix.print(accY, 2);
      } else {
        matrix.print("Z:");
        matrix.print(accZ, 2);
      }
      matrix.setTextColor(matrix.color565(0, 255, 0));
      matrix.setCursor(24, y);
      matrix.print("OK");
    }
  }
  drawNet();
  int16_t yb = SEG_TOP_H + SEG_MID_H + 2;
  if (showAI) {
    matrix.setFont(NULL);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.color565(255, 255, 255));
    matrix.setCursor(2, yb + 2);
    matrix.print("AI:");
    if (millis() % 1000 < 500) {
      matrix.setCursor(16, yb + 2);
      matrix.print("ON");
      matrix.drawRect(15, yb + 1, 14, 10,
                      matrix.color565(255, 255, 255));
    }
  }
  if (showCube) {
    drawTinyCube(matrix.width() - 28, yb + 14);
  }
  matrix.show();
}