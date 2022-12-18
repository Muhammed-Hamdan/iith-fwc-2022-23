#include<Arduino.h>
#define PIN_A 12
#define PIN_B 11
#define PIN_C 10
#define PIN_D 9
//Declaring all variables as integers
int A=0,B=0,C=0,D=0;

//Code released under GNU GPL.  Free to use for anything.
void kmap_convert_7seg(int A, int B, int C, int D)
{
  int a = (!B&&!D) || (!A&&C) || (B&&C) || (A&&!D) || (!A&&B&&D) || (A&&!B&&!C);
  int b = (!A&&!B) || (!B&&!D) || (!A&&!C&&!D) || (!A&&C&&D) || (A&&!C&&D);
  int c = (!A&&!C) || (!A&&D) || (!C&&D) || (!A&&B) || (A&&!B);
  int d = (A&&!C) || (!A&&!B&&!D) || (!B&&C&&D) || (B&&!C&&D) || (B&&C&&!D);
  int e = (!B&&!D) || (C&&!D) || (A&&C) || (A&&B);
  int f = (!C&&!D) || (B&&!D) || (A&&!B) || (A&&C) || (!A&&B&&!C);
  int g = (!B&&C) || (C&&!D) || (A&&!B) || (A&&D) || (!A&&B&&!C);

  digitalWrite(2, !a); //LSB
  digitalWrite(3, !b); 
  digitalWrite(4, !c); 
  digitalWrite(5, !d); 
  digitalWrite(6, !e); 
  digitalWrite(7, !f); 
  digitalWrite(8, !g); //MSB
}
// the setup function runs once when you press reset or power the board
void setup() {
    pinMode(2, OUTPUT);  
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);  
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(PIN_A, INPUT);
    pinMode(PIN_B, INPUT);  
    pinMode(PIN_C, INPUT);
    pinMode(PIN_D, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  A = digitalRead(PIN_A);
  B = digitalRead(PIN_B);
  C = digitalRead(PIN_C);
  D = digitalRead(PIN_D);
  kmap_convert_7seg(A,B,C,D);
  delay(20);
}
//&& is the AND operation
//|| is the OR operation
//! is the NOT operation


/* With don't care (0-9)
  int a = (Y&&!Z&&!W) || (!X&&!Y&&!Z&&W);
  int b = (Y&&!Z&&W) || (Y&&Z&&!W);
  int c = (!Y&&Z&&!W);
  int d = (!Y&&!Z&&W) || (Y&&!Z&&!W) || (Y&&Z&&W);
  int e = (W) || (Y&&!Z);
  int f = (!Y&&Z) || (Z&&W) || (!X&&!Y&&W);
  int g = (!X&&!Y&&!Z) || (Y&&Z&&W);
  */

  /* Without don't care (0-9 A-F) 
  int a = (!X&&!Y&&!Z&&!W) || (!X&&Y&&!Z&&!W) || (X&&!Y&&Z&&W) || (X&&Y&&!Z&&W);
  int b = (Y&&Z&&!W) || (X&&Z&&W) || (X&&Y&&!W) || (!X&&Y&&!Z&&W);
  int c = (X&&Y&&!W) || (X&&Y&&Z) || (!X&&!Y&&Z&&!W);
  int d = (!Y&&!Z&&W) || (Y&&Z&&W) || (!X&&Y&&!Z&&!W) || (X&&!Y&&Z&&!W);
  int e = (!X&&W) || (!Y&&!Z&&W) || (!X&&Y&&!Z);
  int f = (!X&&!Y&&W) || (!X&&!Y&&Z) || (!X&&Z&&W) || (X&&Y&&!Z&&W);
  int g = (!X&&!Y&&!Z) || (!X&&Y&&Z&&W) || (X&&Y&&!Z&&W);
  */

  /* With don't care (0-9)
  int a = (!Z&&!Y&&!X&&W) || (!Z&&Y&&!X&&!W);
  int b = (!Z&&Y&!X&&W) || (!Z&&Y&&X&&!W);
  int c = (!Z&&!Y&&X&&!W);
  int d = (!Z&&!Y&&!W&&X) || (!Z&&Y&&!W&&!X) || (!Z&&Y&&W&&X) || (Z&&!Y&&!W&&X);
  int e = (Z||Y||W||X) && (Z||Y||!W||X) && (Z||!Y||!W||X) && (!Z||Y||W||X);
  int f = (!Z&&!Y&&!W&&X) || (!Z&&!Y&&W&&!X) || (!Z&&!Y&&W&&X) || (!Z&&Y&&W&&X);
  int g = (!Z&&!Y&&!X&&!W) || (!Z&&!Y&&!X&&W) || (!Z&&Y&&X&&W);
  */

  /* New Without don't care (0-9 A-F) 
  int a = (!X&&!Y&&!Z&&!W) || (!X&&Y&&!Z&&!W) || (X&&!Y&&Z&&W) || (X&&Y&&!Z&&W);
  int b = (Y&&Z&&!W) || (X&&Z&&W) || (X&&Y&&!W) || (!X&&Y&&!Z&&W);
  int c = (X&&Y&&!W) || (X&&Y&&Z) || (!X&&!Y&&Z&&!W);
  int d = (!Y&&!Z&&W) || (Y&&Z&&W) || (!X&&Y&&!Z&&!W) || (X&&!Y&&Z&&!W);
  int e = (!X&&W) || (!Y&&!Z&&W) || (!X&&Y&&!Z);
  int f = (!X&&!Y&&W) || (!X&&!Y&&Z) || (!X&&Z&&W) || (X&&Y&&!Z&&W);
  int g = (!X&&!Y&&!Z) || (!X&&Y&&Z&&W) || (X&&Y&&!Z&&W);
  */