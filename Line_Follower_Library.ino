#include "LFR.h"

LineFollower LFR;

void setup() {
  LFR.LeftMotor(4, 2, 3);
  LFR.RightMotor(7, 6, 5);
  LFR.IR_Array(8, 9, 10, 11, 12);
  LFR.ReadyLineFollower();
}

void loop() {
  LFR.basicLFR();
}
