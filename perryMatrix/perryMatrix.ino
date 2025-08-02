// © 2025 SC5K Systems

#include <Arduino.h>
#include "src/matrix_config.h"
#include "src/globals.h"
#include "src/checklist.h"
#include "src/boot_sequence.h"
#include "src/autonomous.h"
#include "src/dynamic.h"

void setup() {
  // USB & RoboRIO serial
  Serial.begin(115200);
  Serial1.begin(9600);
  delay(500);
  Serial.setTimeout(2000);
  Serial1.setTimeout(2000);
  if (Serial) {
    Serial.println("USB serial active");
    Serial.println("RoboRIO serial active");
  }

  // Matrix init
  if (matrix.begin() != PROTOMATTER_OK) while (1) delay(10);
  matrix.setRotation(1);
  matrix.setTextWrap(false);
  matrix.setTextSize(1);

  // allocate the shared buffer used by readAndProcess()
  buf = new char[32];

  // one‐time boot sequence (splash, color test, write-on checklist)
  initBootSequence();
}

void loop() {
  // 1) read/dispatch incoming messages from RoboRIO or USB
  handleRobotMessage();

  // 2) run per‐mode logic
  switch (currentMode) {
    case MODE_CHECKLIST:
      // checklist mode: update or animate checklist and subsequent scroller/perry/audio
      runBootSequence();
      break;

    case MODE_AUTONOMOUS:
      // autonomous mode: show AUTO LOCK animation until duration expires
      if (autoActive) runAutonomousFrame();
      break;

    case MODE_DYNAMIC:
      // dynamic mode: continuously render network + accel/AI/cube as enabled
      runDynamicFrame();
      break;

    default:
      // MODE_NULL or any undefined mode: remain blank
      break;
  }
}
