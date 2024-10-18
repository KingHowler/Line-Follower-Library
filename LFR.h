class Motor {
  public : int F;
  public : int B;
  public : int S;

  public : void ReadyMotor() {
    pinMode(F, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(S, OUTPUT);
  }

  public : void turnMotor(bool forward, int Speed) {
    digitalWrite(F, forward);
    digitalWrite(B, !forward);
    analogWrite(S, Speed);
  }
};

class IR_Array {
  public :  int LH;
  public :  int LM;
  public :  int  M;
  public :  int RM;
  public :  int RH;

  public : bool LH_V;
  public : bool LM_V;
  public : bool M_V;
  public : bool RM_V;
  public : bool RH_V;

  public : void ReadyIR() {
    pinMode(LH, INPUT);
    pinMode(LM, INPUT);
    pinMode( M, INPUT);
    pinMode(RM, INPUT);
    pinMode(RH, INPUT);
  }

  public : void Update(int LookFor) {
    LH_V = (digitalRead(LH) == LookFor);
    LM_V = (digitalRead(LM) == LookFor);
    M_V =  (digitalRead(M) == LookFor);
    RM_V = (digitalRead(RM) == LookFor);
    RH_V = (digitalRead(RH) == LookFor);
  }

  void PrintSensors() {
    Serial.print(LH_V);
    Serial.print("   ");
    Serial.print(LM_V);
    Serial.print("   ");
    Serial.print(M_V);
    Serial.print("   ");
    Serial.print(RM_V);
    Serial.print("   ");
    Serial.println(RH_V);
}
};

class LineFollower {

  public : Motor Left;
  public : Motor Right;

  public : IR_Array IR;
  
  public : void LeftMotor(int Forward_Pin, int Backward_Pin, int Speed_Pin) {
    Left.F = Forward_Pin;
    Left.B = Backward_Pin;
    Left.S = Speed_Pin;
  }

  public :  void RightMotor(int Forward_Pin, int Backward_Pin, int Speed_Pin) {
    Right.F = Forward_Pin;
    Right.B = Backward_Pin;
    Right.S = Speed_Pin;
  }

  public :  void IR_Array(int Left_Highest, int Left_Middle, int Middle, int Right_Middle, int Right_Highest) {
    IR.LH = Left_Highest;
    IR.LM = Left_Middle;
    IR.M  = Middle;
    IR.RM = Right_Middle;
    IR.RH = Right_Highest;
  }

  public : void ReadyLineFollower() {
    Left.ReadyMotor();
    Right.ReadyMotor();
    IR.ReadyIR();
  }

  public : void forward(int L_Speed, int R_Speed) {
    Left.turnMotor(1, L_Speed);
    Right.turnMotor(1, R_Speed);
  }

  public : void left(int L_Speed, int R_Speed) {
    Left.turnMotor(0, L_Speed);
    Right.turnMotor(1, R_Speed);
  }

  public : void right(int L_Speed, int R_Speed) {
    Left.turnMotor(1, L_Speed);
    Right.turnMotor(0, R_Speed);
  }

  public : void backward(int L_Speed, int R_Speed) {
    Left.turnMotor(0, L_Speed);
    Right.turnMotor(0, R_Speed);
  }

  public : void stop() {
    Left.turnMotor(1, 0);
    Right.turnMotor(1, 0);
  }

  public : void basicLFR(int MaxSpeed, int SmallTurnSpeed) {
    // Small Deviations
    if (IR.LM_V && !IR.LH_V){ left(SmallTurnSpeed, SmallTurnSpeed); Serial.println("Small Left"); return; }
    if (IR.RM_V && !IR.RH_V){ right(SmallTurnSpeed, SmallTurnSpeed); Serial.println("Small Right"); return; }

    // 90 Degrees
    if (IR.M_V && IR.LM_V &&  IR.LH_V) { left(MaxSpeed, MaxSpeed); Serial.println("Left 90"); return; }
    if (IR.M_V && IR.RM_V &&  IR.RH_V) { right(MaxSpeed, MaxSpeed); Serial.println("Right 90"); return; }

    // Offshoots
    if (IR.LH_V){ left(SmallTurnSpeed, MaxSpeed); Serial.println("Big Left"); return; }
    if (IR.RH_V){ right(MaxSpeed, SmallTurnSpeed); Serial.println("Big Right"); return; }

    // Straight if no errors found
    if (IR.M_V && !IR.LH_V && !IR.RH_V && ! IR.LM_V && !IR.RM_V) { forward(MaxSpeed, MaxSpeed); Serial.println("Forward"); }
  }

  public : void P_LFR(int P) {
    float Error = ( (IR.LH_V*-2) + (IR.LM_V*-1) + (IR.M_V*0) + (IR.RM_V*1) + (IR.RH_V*2) )   /   (!IR.LH_V + !IR.LM_V + !IR.M_V + !IR.RM_V + !IR.RH_V);
    int speeds = min(abs(P*Error), 255);

    if (Error <  0) left     (speeds, speeds);
    if (Error == 0) forward  (speeds, speeds);
    if (Error >  0) right    (speeds, speeds);
  }
};