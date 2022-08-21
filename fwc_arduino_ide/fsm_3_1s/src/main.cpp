#include<Arduino.h>
#define X0_PIN  11   // The BIT-STREAM
#define X1_PIN  6    // 
#define X2_PIN  7    //
#define X3_PIN  8    // 
#define X4_PIN  9    //
#define S0_PIN  2 //
#define S1_PIN  3 //
#define S2_PIN  4 //
#define S3_PIN  5 //
#define OUT_PIN 11
#define CLK_PIN 10 //
#define CLK_TP  4000

int X0, X1, X2, X3, X4, S0, S1, S2, S3, out;

void write_7447(int bcd){
  digitalWrite(A0, bcd&(0x01<<0));
  digitalWrite(A1, bcd&(0x01<<1));
  digitalWrite(A2, bcd&(0x01<<2));
  digitalWrite(A3, bcd&(0x01<<3));
}

void fsm_read()
{
  X0 = digitalRead(X0_PIN);
  X1 = digitalRead(X1_PIN);
  X2 = digitalRead(X2_PIN);
  X3 = digitalRead(X3_PIN);
  X4 = digitalRead(X4_PIN);
}

void fsm_update()
{
  // Without overlap
  /*
  out = (X3 && !X0); 
  S3 = (X3 && !X0);
  S2 = (!X4 && X2 && X0);
  S1 = (X1 && !X0);
  S0 = (!X1 && X0);
  */

  // With overlap
  out = (X3 && !X0); 
  S3 = (X3 && !X0);
  S2 = (X2 && X0);
  S1 = (X1 && !X0);
  S0 = (!X1 && X0);

  //digitalWrite(OUT_PIN, out);
  if(out) write_7447(1);
  else write_7447(0);
  digitalWrite(S0_PIN, S0);
  digitalWrite(S1_PIN, S1);
  digitalWrite(S2_PIN, S2);
  digitalWrite(S3_PIN, S3);
}

void clock_tick()
{
  digitalWrite(CLK_PIN, 1);
  digitalWrite(13, 1);
  delay(CLK_TP/2);
  digitalWrite(CLK_PIN, 0);
  digitalWrite(13, 0);
  delay(CLK_TP/2);
}

void setup() {
    pinMode(S0_PIN, OUTPUT);   
    pinMode(S1_PIN, OUTPUT);  
    pinMode(S2_PIN, OUTPUT);  
    pinMode(S3_PIN, OUTPUT);  
    //pinMode(OUT_PIN, OUTPUT); 
    pinMode(X0_PIN, INPUT);   
    pinMode(X1_PIN, INPUT);  
    pinMode(X2_PIN, INPUT);
    pinMode(X3_PIN, INPUT);
    pinMode(X4_PIN, INPUT);
    pinMode(CLK_PIN, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(13, OUTPUT);
}


void loop() {
  fsm_read();
  fsm_update();
  clock_tick();
}


/* Code for 3 ones overlapping 
  out = (X2 && !X1 && X0) || (X2 && X1 && X0); 
  S0 =  (!X2 && X1 && X0) || (X2 && !X1 && X0) || (X2 && X1 && X0); // Non overlap - remove last bracket 
  S1 =  (!X1 && X0) || (X2 && X0) || (X2 && X1 && X0); // Non overlap - remove last bracket
  fsm_output(out, S0, S1);
  digitalWrite(7, 1);
  delay(25);
  digitalWrite(7, 0);
  delay(25);
  X0 = digitalRead(4);
  X1 = digitalRead(3);
  X2 = digitalRead(2); 
  */