// © 2025 SC5K Systems

#include "src/checklist.h"
#include "src/matrix_config.h"
#include "src/globals.h"
#include "src/helpers.h"
#include <Arduino.h>
#include <string.h>

/*/
drawDashedOutline
iterate perimeter, drawing pixel every 3 steps, wrapping around edges
/*/
void drawDashedOutline(int bx, int by, int bw, int bh,
                       int perim, int offset, uint16_t color) {
  for (int pos = 0; pos < perim; pos++) {
    if ((pos + offset) % 3 < 2) {
      int x, y;
      if (pos < bw) {
        x = bx + pos; y = by;
      } else if (pos < bw + bh - 1) {
        x = bx + bw - 1; y = by + (pos - bw + 1);
      } else if (pos < bw*2 + bh - 2) {
        x = bx + (bw - 1) - (pos - (bw + bh - 2));
        y = by + bh - 1;
      } else {
        x = bx;
        y = by + (bh - 1) - (pos - (bw*2 + bh - 3));
      }
      matrix.drawPixel(x, y, color);
    }
  }
}

/*/
sweepBoxLR
fill a vertical bar segment of box idx from left to right in color
/*/
void sweepBoxLR(uint8_t idx, uint16_t color) {
  uint16_t sw = matrix.width();
  int16_t lx = (sw - chkBoxW)/2;
  int16_t ly = topSpacing + CHAR_H + betweenSetupAndPiece + pieceYOffset
               + idx*(CHAR_H + textBoxGap + chkBoxH + itemGap);
  for (uint8_t c = 0; c < chkBoxW; c++) {
    matrix.drawFastVLine(lx + c, ly + CHAR_H + textBoxGap, chkBoxH, color);
    matrix.show();
    delay(boxDelay / chkBoxW);
  }
}

/*/
sweepBoxRL
fill a vertical bar segment of box idx from right to left in color
/*/
void sweepBoxRL(uint8_t idx, uint16_t color) {
  uint16_t sw = matrix.width();
  int16_t lx = (sw - chkBoxW)/2;
  int16_t ly = topSpacing + CHAR_H + betweenSetupAndPiece + pieceYOffset
               + idx*(CHAR_H + textBoxGap + chkBoxH + itemGap);
  for (int16_t c = chkBoxW - 1; c >= 0; c--) {
    matrix.drawFastVLine(lx + c, ly + CHAR_H + textBoxGap, chkBoxH, color);
    matrix.show();
    delay(boxDelay / chkBoxW);
  }
}

/*/
drawChecklistStatic
clear screen, draw “MATCH SETUP” header, each item + colored box,
and bottom “NOT READY” if not all checked
/*/
void drawChecklistStatic() {
  matrix.fillScreen(0);
  matrix.setTextColor(matrix.color565(255,255,255));

  // header
  matrix.setCursor((matrix.width() - 5*CHAR_W)/2, topSpacing);
  matrix.print("MATCH");
  matrix.setCursor((matrix.width() - 5*CHAR_W)/2, topSpacing + CHAR_H);
  matrix.print("SETUP");

  // items + boxes
  for (uint8_t i = 0; i < numChecklist; i++) {
    const char* L = checklistItems[i];
    int16_t ln = strlen(L);
    int16_t ty = topSpacing + CHAR_H + betweenSetupAndPiece + pieceYOffset
               + i*(CHAR_H*2 + textBoxGap + itemGap);
    int16_t lx = (matrix.width() - ln*CHAR_W)/2;
    matrix.setCursor(lx, ty);
    matrix.print(L);

    int16_t bx = (matrix.width() - chkBoxW)/2;
    int16_t by = ty + CHAR_H + textBoxGap;
    uint16_t col = (prevChecklist[i] == 1)
                  ? matrix.color565(0,255,0)
                  : matrix.color565(255,0,0);
    matrix.fillRect(bx, by, chkBoxW, chkBoxH, col);
  }

  // bottom status
  if (!gReadyState) {
    matrix.setTextColor(matrix.color565(255,0,0));
    matrix.setCursor((matrix.width()-3*CHAR_W)/2,
                     matrix.height()-CHAR_H*2);
    matrix.print("NOT");
    matrix.setCursor((matrix.width()-5*CHAR_W)/2,
                     matrix.height()-CHAR_H);
    matrix.print("READY");
  }

  matrix.show();
}

/*/
processChecklistPayload
parse payload CSV of 0/1 into states[], handle immediate redraw
on drop from all-green, then detect changed boxes, animate them,
and flash bottom NOT/READY based on new state
/*/
void processChecklistPayload(const char* payload) {
  // parse CSV
  int states[numChecklist] = {0};
  char tmp[32];
  strncpy(tmp, payload, sizeof(tmp)-1);
  tmp[sizeof(tmp)-1] = '\0';
  char *p = tmp;
  for (uint8_t i = 0; i < numChecklist; i++) {
    states[i] = atoi(p);
    char *comma = strchr(p, ',');
    if (!comma) break;
    p = comma + 1;
  }

  // handle drop from scroller to not-ready baseline
  bool newAllGreen = true;
  for (uint8_t i = 0; i < numChecklist; i++) {
    if (states[i] != 1) { newAllGreen = false; break; }
  }
  if (sponsorLaunched && !newAllGreen) {
    sponsorLaunched = false;
    readyTimestamp  = 0;
    // draw full green checklist baseline
    matrix.fillScreen(0);
    matrix.setTextColor(matrix.color565(255,255,255));
    matrix.setCursor((matrix.width()-5*CHAR_W)/2, topSpacing);
    matrix.print("MATCH");
    matrix.setCursor((matrix.width()-5*CHAR_W)/2, topSpacing+CHAR_H);
    matrix.print("SETUP");
    for (uint8_t i = 0; i < numChecklist; i++) {
      const char* L = checklistItems[i];
      int16_t ln = strlen(L);
      int16_t ty = topSpacing + CHAR_H + betweenSetupAndPiece + pieceYOffset
                   + i*(CHAR_H*2 + textBoxGap + itemGap);
      int16_t lx = (matrix.width()-ln*CHAR_W)/2;
      matrix.setCursor(lx, ty);
      matrix.print(L);
      int16_t bx = (matrix.width()-chkBoxW)/2;
      int16_t by = ty + CHAR_H + textBoxGap;
      matrix.fillRect(bx, by, chkBoxW, chkBoxH,
                      matrix.color565(0,255,0));
      prevChecklist[i] = 1;
    }
    matrix.setTextColor(matrix.color565(0,255,0));
    matrix.setCursor((matrix.width()-5*CHAR_W)/2,
                     matrix.height()-CHAR_H);
    matrix.print("READY");
    matrix.show();
    // continue to animate below
  }

  // detect changes
  bool allGreen = true;
  uint8_t changedCount = 0;
  uint8_t changedIdx[numChecklist];
  uint16_t changedCol[numChecklist];
  for (uint8_t i = 0; i < numChecklist; i++) {
    if (states[i] != prevChecklist[i]) {
      changedIdx[changedCount] = i;
      changedCol[changedCount] = (states[i]==1)
        ? matrix.color565(0,255,0)
        : matrix.color565(255,0,0);
      prevChecklist[i] = states[i];
      changedCount++;
    }
    if (prevChecklist[i] == 0) allGreen = false;
  }

  // animate box fills
  if (changedCount) {
    for (uint8_t step = 0; step < chkBoxW; step++) {
      for (uint8_t k = 0; k < changedCount; k++) {
        uint8_t idx = changedIdx[k];
        uint16_t col = changedCol[k];
        int16_t sw = matrix.width();
        int16_t lx = (sw - chkBoxW)/2;
        int16_t ly = topSpacing + CHAR_H + betweenSetupAndPiece + pieceYOffset
                      + idx*(CHAR_H + textBoxGap + chkBoxH + itemGap);
        uint8_t xOff = (col==matrix.color565(0,255,0))
                      ? chkBoxW-1-step : step;
        matrix.drawFastVLine(lx+xOff, ly+CHAR_H+textBoxGap, chkBoxH, col);
      }
      matrix.show();
      delay(boxDelay/chkBoxW);
    }
  }

  // flash bottom NOT/READY
  uint16_t white=matrix.color565(255,255,255), black=0,
           green=matrix.color565(0,255,0),
           red=matrix.color565(255,0,0);
  int16_t sw=matrix.width(),
          y4=matrix.height()-CHAR_H*2;
  if (allGreen && !gReadyState) {
    // flash READY
    const char* n="NOT"; int16_t xNot=(sw-3*CHAR_W)/2;
    for (uint8_t j=0;j<3;j++){
      matrix.fillRect(xNot+j*CHAR_W,y4,CHAR_W,CHAR_H,white);
      matrix.show(); delay(typeDelay);
      matrix.fillRect(xNot+j*CHAR_W,y4,CHAR_W,CHAR_H,black);
      matrix.show();
    }
    const char* r="READY"; int16_t xR=(sw-5*CHAR_W)/2;
    for (uint8_t j=0;j<5;j++){
      int16_t cx=xR+j*CHAR_W;
      matrix.fillRect(cx,y4+CHAR_H,CHAR_W,CHAR_H,white);
      matrix.show(); delay(typeDelay);
      matrix.fillRect(cx,y4+CHAR_H,CHAR_W,CHAR_H,black);
      matrix.setTextColor(green);
      matrix.setCursor(cx,y4+CHAR_H);
      matrix.print(r[j]);
      matrix.show();
    }
    gReadyState=true;
  } else if (!allGreen && gReadyState) {
    // flash NOT
    const char* n="NOT"; int16_t xNot=(sw-3*CHAR_W)/2;
    for (uint8_t j=0;j<3;j++){
      int16_t cx=xNot+j*CHAR_W;
      matrix.fillRect(cx,y4,CHAR_W,CHAR_H,white);
      matrix.show(); delay(typeDelay);
      matrix.fillRect(cx,y4,CHAR_W,CHAR_H,black);
      matrix.setTextColor(red);
      matrix.setCursor(cx,y4);
      matrix.print(n[j]);
      matrix.show();
    }
    const char* r="READY"; int16_t xR=(sw-5*CHAR_W)/2;
    for (uint8_t j=0;j<5;j++){
      int16_t cx=xR+j*CHAR_W;
      matrix.fillRect(cx,y4+CHAR_H,CHAR_W,CHAR_H,white);
      matrix.show(); delay(typeDelay);
      matrix.fillRect(cx,y4+CHAR_H,CHAR_W,CHAR_H,black);
      matrix.setTextColor(red);
      matrix.setCursor(cx,y4+CHAR_H);
      matrix.print(r[j]);
      matrix.show();
    }
    gReadyState=false;
  }
}

/*/
readAndProcess
grab all available bytes until '\\n' into buf, null-terminate, log, then forward to processing
/*/
void readAndProcess(Stream &in, const char *label) {
  if (!in.available()) return;
  int len = 0;
  while (in.available()) {
    len = in.readBytesUntil('\n', buf, sizeof(buf)-1);
  }
  if (len <= 0) return;
  buf[len] = '\0';
  Serial.print("received on ");
  Serial.print(label);
  Serial.print(": ");
  Serial.println(buf);
  processChecklistPayload(buf);
}
