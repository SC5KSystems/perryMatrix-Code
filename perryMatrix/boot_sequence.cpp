// © 2025 SC5K Systems

#include "src/boot_sequence.h"
#include "src/checklist.h"
#include "src/helpers.h"
#include "src/globals.h"
#include "src/matrix_config.h"
#include "src/audio_vis.h"
#include "src/sponsor_scroller.h"
#include "src/perry_loader.h"
#include "src/autonomous.h"
#include "src/dynamic.h"
#include <Arduino.h>

/*/
initBootSequence
print serial banners, show boot/mode text, animate type options,
draw progressive rectangle outline, blink “LED”, include
a commented-out color test, then write on checklist phase
/*/
void initBootSequence() {
  Serial.println("USB serial active");
  Serial.println("RoboRIO serial active");
  uint16_t sw = matrix.width(), sh = matrix.height();

  // boot mode splash
  matrix.fillScreen(0);
  const char *b1 = "BOOT", *b2 = "MODE";
  int16_t x1 = (sw - strlen(b1)*CHAR_W)/2;
  int16_t x2 = (sw - strlen(b2)*CHAR_W)/2;
  matrix.setTextColor(matrix.color565(255,255,255));
  matrix.setCursor(x1, CHAR_H);
  matrix.print(b1);
  matrix.setCursor(x2, CHAR_H*2);
  matrix.print(b2);
  matrix.show(); delay(bootDelay);

  // type options animation
  const char *opts[] = {"LED","ROBOT","USB_d"};
  for (uint8_t i=0; i<3; i++){
    uint8_t L = strlen(opts[i]);
    int16_t ox = (sw - L*CHAR_W)/2;
    int16_t oy = (sh - CHAR_H*6)/2 + i*CHAR_H*2;
    for (uint8_t j=0; j<L; j++){
      matrix.fillRect(ox+j*CHAR_W, oy, CHAR_W, CHAR_H,
                      matrix.color565(0,255,0));
      matrix.show(); delay(typeDelay);
      matrix.fillRect(ox+j*CHAR_W, oy, CHAR_W, CHAR_H, 0);
      matrix.setTextColor(matrix.color565(0,255,0));
      matrix.setCursor(ox+j*CHAR_W, oy);
      matrix.print(opts[i][j]);
      matrix.show();
    }
  }
  delay(postOptionsDelay);

  // progressive outline draw
  uint8_t L0 = strlen(opts[0]);
  int16_t ox0 = (sw - L0*CHAR_W)/2, oy0 = (sh - CHAR_H*6)/2;
  int16_t bx = ox0-1, by = oy0-2;
  int16_t bw = L0*CHAR_W+1, bh = CHAR_H+3;
  uint16_t green = matrix.color565(0,255,0);
  int hLen=bw-1, vLen=bh-1, steps=hLen+vLen,
      delayStep=350/steps;
  matrix.drawPixel(bx, by, green); matrix.show(); delay(delayStep);
  for (int s=1; s<=steps; s++){
    if (s<=hLen) matrix.drawPixel(bx+s, by, green);
    else          matrix.drawPixel(bx+bw-1, by+(s-hLen), green);
    if (s<=vLen) matrix.drawPixel(bx, by+s, green);
    else          matrix.drawPixel(bx+(s-vLen), by+bh-1, green);
    matrix.show(); delay(delayStep);
  }
  delay(dashDelayBefore);
  int perim = 2*(bw+bh)-4, offset=0;
  unsigned long dashStart=millis();
  while (millis()-dashStart < dashedPhaseDuration){
    matrix.drawRect(bx,by,bw,bh,0);
    drawDashedOutline(bx,by,bw,bh,perim,offset,green);
    matrix.show(); delay(flashDelay);
    offset=(offset+1)%perim;
  }

  // blink "LED"
  for (uint8_t f=0; f<flashCount; f++){
    matrix.fillRect(ox0,oy0,bw,CHAR_H,0);
    offset=(offset+1)%perim;
    matrix.drawRect(bx,by,bw,bh,0);
    drawDashedOutline(bx,by,bw,bh,perim,offset,green);
    matrix.show(); delay(flashDelay);
    matrix.setTextColor(green);
    matrix.setCursor(ox0,oy0);
    matrix.print(opts[0]);
    offset=(offset+1)%perim;
    matrix.drawRect(bx,by,bw,bh,0);
    drawDashedOutline(bx,by,bw,bh,perim,offset,green);
    matrix.show(); delay(flashDelay);
  }

  // color test (commented out block)
  /*/
  {
    const char *c1="COLOR", *c2="TEST";
    int16_t cx1=(sw-strlen(c1)*CHAR_W)/2, cy1=sh/2-CHAR_H,
            cx2=(sw-strlen(c2)*CHAR_W)/2, cy2=sh/2;
    for (uint8_t p=0; p<3; p++){
      uint8_t pr=(p==0)*255, pg=(p==1)*255, pb=(p==2)*255;
      uint8_t invR=255-pr, invG=255-pg, invB=255-pb;
      unsigned long start=millis();
      uint16_t total=sw*sh;
      uint16_t *coords=new uint16_t[total];
      for (uint16_t i=0;i<total;i++) coords[i]=i;
      shuffleArray(coords,total);
      for (uint16_t i=0;i<total;i++){
        matrix.drawPixel(coords[i]%sw,coords[i]/sw,
                         matrix.color565(pr,pg,pb));
        if ((i&0x1F)==0){
          float t=float(millis()-start)/float(hueCycleDuration);
          if (t>1.0) t=1.0;
          uint8_t tr=uint8_t(255+(invR-255)*t),
                  tg=uint8_t(255+(invG-255)*t),
                  tb=uint8_t(255+(invB-255)*t);
          matrix.setTextColor(matrix.color565(tr,tg,tb));
          matrix.setCursor(cx1,cy1); matrix.print(c1);
          matrix.setCursor(cx2,cy2); matrix.print(c2);
          matrix.show();
        }
      }
      shuffleArray(coords,total);
      for (uint16_t i=0;i<total;i++){
        matrix.drawPixel(coords[i]%sw,coords[i]/sw,0);
        if ((i&0x1F)==0) matrix.show();
      }
      delete[] coords;
    }
  }
  /*/

  // checklist phase write-on
  matrix.fillScreen(0); delay(100);
  matrix.setTextColor(matrix.color565(255,255,255));
  matrix.setCursor((sw-5*CHAR_W)/2, topSpacing);
  matrix.print("MATCH");
  matrix.setCursor((sw-5*CHAR_W)/2, topSpacing+CHAR_H);
  matrix.print("SETUP");
  matrix.show(); delay(1000);
  for (uint8_t i=0; i<numChecklist; i++){
    const char* L = checklistItems[i];
    int16_t ln = strlen(L);
    int16_t ty = topSpacing+CHAR_H+betweenSetupAndPiece+pieceYOffset
               + i*(CHAR_H*2+textBoxGap+itemGap);
    int16_t lx = (sw-ln*CHAR_W)/2;
    for (uint8_t j=0; j<ln; j++){
      int16_t cx = lx + j*CHAR_W;
      matrix.fillRect(cx,ty,CHAR_W,CHAR_H,
                      matrix.color565(255,255,255));
      matrix.show(); delay(typeDelay);
      matrix.fillRect(cx,ty,CHAR_W,CHAR_H,0);
      matrix.setTextColor(matrix.color565(255,255,255));
      matrix.setCursor(cx,ty);
      matrix.print(L[j]);
      matrix.show();
    }
    sweepBoxLR(i, matrix.color565(255,0,0));
  }
  delay(postChecklistDelay);
  matrix.setTextColor(matrix.color565(255,0,0));
  matrix.setCursor((sw-3*CHAR_W)/2, sh-CHAR_H*2);
  matrix.print("NOT");
  matrix.setCursor((sw-5*CHAR_W)/2, sh-CHAR_H);
  matrix.print("READY");
  matrix.show();
}

/*/
runBootSequence
in checklist mode: read/process serial, if not ready draw static,
else sequence audio, sponsor, perry, audio until state changes
/*/
void runBootSequence() {
  // read incoming checklist updates
  readAndProcess(Serial,  "USB");
  readAndProcess(Serial1, "RoboRIO");

  // if not ready, reset everything and redraw static checklist
  if (!gReadyState) {
    sponsorLaunched = perryActive = audioActive = false;
    readyTimestamp = lastSponsor = 0;
    drawChecklistStatic();
    return;
  }

  // if audio‐vis is running, delegate to it
  if (audioActive) {
    runAudioVisFrame();
    return;
  }

  // sponsor → perry → audio sequence
  if (gReadyState && !sponsorLaunched) {
    // wait 10s after ready to launch sponsor scroller
    if (readyTimestamp == 0) {
      readyTimestamp = millis();
    } else if (millis() - readyTimestamp >= 10000UL) {
      sponsorLaunched = true;
      initSponsorScroller();
    }

  } else if (sponsorLaunched && !perryActive) {
    // run sponsor scroller until cycle wraps, then start Perry loader
    runSponsorScroller();
    if (lastSponsor == sponsorCount - 1 && currentSponsor == 0) {
      perryActive = true;
      initPerryLoader();
    }
    lastSponsor = currentSponsor;

  } else if (perryActive) {
    // Perry loader write → obfuscation → decryption → hold → audio
    if (p_writing) {
      writeEncryptedText4();

    } else if (!p_decrypting) {
      if (millis() >= p_startObf + ODUR) {
        p_decrypting = true;
        p_lastUpdate = millis();
      } else if (millis() - p_lastObf > OSPD) {
        displayRandomText4();
        p_lastObf = millis();
      }

    } else if (p_linesDone < perryLineCount4) {
      // continue decryption animation
      if (millis() - p_lastUpdate > ADEL) {
        updateDecryption4();
      }

    } else {
      // all lines decrypted: hold full text for 2s, then start audio‐vis
      if (p_finalHold == 0) {
        p_finalHold = millis();
      } else if (millis() - p_finalHold >= 3000UL) {
        if (!audioActive) {
          perryActive = false;
          audioActive = true;
          initAudioVis();
        }
      }
    }
  }
}

/*/
handleRobotMessage
(see header doc)
/*/
void handleRobotMessage() {
  Stream* port = nullptr;
  if      (Serial1.available()) port=&Serial1;
  else if (Serial.available())  port=&Serial;
  else return;
  int len = port->readBytesUntil('\n', lineBuf, sizeof(lineBuf)-1);
  if (len<=0) return;
  lineBuf[len]='\0';
  Serial.print("Msg from ");
  Serial.print((port==&Serial1)?"Serial1":"Serial");
  Serial.print(": "); Serial.println(lineBuf);
  char* tok = strtok(lineBuf," ");
  if (!tok) return;
  int8_t newMode = atoi(tok);
  char* payload = strtok(nullptr,"");
  if (newMode!=currentMode) {
    lastMode=currentMode;
    currentMode=Mode(newMode);
    audioActive=sponsorLaunched=perryActive=false;
    if (currentMode==MODE_CHECKLIST && payload)
      processChecklistPayload(payload);
    else if (currentMode==MODE_AUTONOMOUS)
      initAutonomous();
    else if (currentMode==MODE_DYNAMIC) {
      initDynamic();
      if (payload) updateDynamicFromPayload(payload);
    } else {
      matrix.fillScreen(0); matrix.show();
    }
  } else if (currentMode==MODE_CHECKLIST && payload) {
    processChecklistPayload(payload);
  } else if (currentMode==MODE_DYNAMIC && payload) {
    updateDynamicFromPayload(payload);
  }
}
