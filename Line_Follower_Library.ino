#include "LFR.h"

LineFollower LFR;

void setup() {
  LFR.RightMotor(4, 2, 3);
  LFR.LeftMotor(7, 6, 5);
  LFR.IR_Array(12, 11, 10, 9, 8);
  LFR.ReadyLineFollower();
  Serial.begin(9600);
}

void loop() {
  LFR.IR.Update(0);
  LFR.basicLFR(200, 150);
}
