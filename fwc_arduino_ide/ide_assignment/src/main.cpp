#include<Arduino.h>
#define A_PIN  2
#define B_PIN  3     
#define C_PIN  4    
#define D_PIN  5  
#define X_PIN  6  // IN-bitstream

#define P_PIN  8 
#define Q_PIN  9 
#define R_PIN  10 
#define S_PIN  11 
#define Z_PIN  12 // OUT-bitstream

#define CLK_PIN 13 
#define CLK_TP  4000

int A, B, C, D, X, P, Q, R, S, Z;

void fsm_read()
{
  A = digitalRead(A_PIN);
  B = digitalRead(B_PIN);
  C = digitalRead(C_PIN);
  D = digitalRead(D_PIN);
  X = digitalRead(X_PIN);
}

void fsm_update()
{
  P = (B&&!D) || (B&&!C&&!X) || (!B&&C&&D&&!X);
  Q = (!B&&C&&!D) || (!B&&C&&X) || (!A&&!B&&!C&&!X) || (B&&!C&&D&&X);
  R = (!B&&D&&X) || (!C&&D&&X) || (A&&!D&&!X) || (!A&&!B&&!C&&X) || (!B&&C&&!D&&!X);
  S = (!A&&!B&&X) || (!B&&!D&&X) || (B&&!D&&!X) || (B&&C&&!X) || (!A&&!C&&D&&X);
  Z = (C&&!D) || (B&&D) || (A&&!D) || (A&&X) || (!A&&!C&&D);
  digitalWrite(P_PIN, P);
  digitalWrite(Q_PIN, Q);
  digitalWrite(R_PIN, R);
  digitalWrite(S_PIN, S);
  digitalWrite(Z_PIN, Z);
}

void clock_tick()
{
  digitalWrite(CLK_PIN, 0);
  delay(CLK_TP/2);
  digitalWrite(CLK_PIN, 1);
  delay(CLK_TP/2);
}

void setup() {
    pinMode(P_PIN, OUTPUT);   
    pinMode(Q_PIN, OUTPUT);  
    pinMode(R_PIN, OUTPUT);  
    pinMode(S_PIN, OUTPUT);  
    pinMode(Z_PIN, OUTPUT);
    pinMode(CLK_PIN, OUTPUT);

    pinMode(A_PIN, INPUT);   
    pinMode(B_PIN, INPUT);  
    pinMode(C_PIN, INPUT);
    pinMode(D_PIN, INPUT);
    pinMode(X_PIN, INPUT);

    digitalWrite(Z_PIN, 0);
}


void loop() {
  fsm_read();
  fsm_update();
  clock_tick();
}