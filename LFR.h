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

  public : int LH_V;
  public : int LM_V;
  public : int  M_V;
  public : int RM_V;
  public : int RH_V;

  public : void ReadyIR() {
    pinMode(LH, INPUT);
    pinMode(LM, INPUT);
    pinMode( M, INPUT);
    pinMode(RM, INPUT);
    pinMode(RH, INPUT);
  }

  public : void Update() {
    LH_V = digitalRead(LH);
    LM_V = digitalRead(LM);
    M_V = digitalRead(M);
    RM_V = digitalRead(RM);
    RH_V = digitalRead(RH);
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

  public : void basicLFR() {
    IR.Update();

    // Small Deviations
    if (IR.LM_V && !IR.LH_V){ left(150, 180); return; }
    if (IR.RM_V && !IR.RH_V){ right(150, 180); return; }

    // Offshoots
    if (IR.LH_V){ left(255, 0); return; }
    if (IR.RH_V){ right(255, 0); return; }

    // 90 Degrees
    if (IR.M_V && IR.LM_V &&  IR.LH_V) { left(255, 255); return; }
    if (IR.M_V && IR.RM_V &&  IR.RH_V) { right(255, 255); return; }

    // Straight if no errors found
    forward(255, 255);
  }

  public : void P_LFR(int P) {
    IR.Update();

    float Error = ( (!IR.LH_V*-2) + (!IR.LM_V*-1) + (!IR.M_V*0) + (!IR.RM_V*1) + (!IR.RH_V*2) )   /   (IR.LH_V + IR.LM_V + IR.M_V + IR.RM_V + IR.RH_V);
    int speeds = min(abs(P*Error), 255);

    if (Error <  0) left     (speeds, speeds);
    if (Error == 0) forward  (speeds, speeds);
    if (Error >  0) right    (speeds, speeds);
  }
};