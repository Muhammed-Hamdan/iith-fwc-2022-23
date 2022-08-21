#include<Arduino.h>
//Declaring all variables as integers
int Z=0,Y=0,X=0,W=0;
int D,C,B,A;

//Code released under GNU GPL.  Free to use for anything.
void disp_7474(int D, int C, int B, int A)
{
  digitalWrite(2, A); //LSB
  digitalWrite(3, B); 
  digitalWrite(4, C); 
  digitalWrite(5, D); //MSB

}
// the setup function runs once when you press reset or power the board
void setup() {
    pinMode(2, OUTPUT);  
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, INPUT);  
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    pinMode(10, OUTPUT);
    digitalWrite(10,0);
}

// the loop function runs over and over again forever
void loop() {
  A = !W;
  B = (W&&!X&&!Y&&!Z) || (!W&&X&&!Y&&!Z) || (W&&!X&&Y&&!Z) || (!W&&X&&Y&&!Z);
  C = (W&&X&&!Y&&!Z) || (!W&&!X&&Y&&!Z) || (W&&!X&&Y&&!Z) || (!W&&X&&Y&&!Z);
  D = (W&&X&&Y&&!Z) || (!W&&!X&&!Y&&Z);
  disp_7474(D,C,B,A);
  digitalWrite(10, 1);
  delay(50);
  digitalWrite(10, 0);
  delay(50);
  W = digitalRead(6);
  X = digitalRead(7); 
  Y = digitalRead(8); 
  Z = digitalRead(9);  
}
//&& is the AND operation
// || is the OR operation
// ! is the NOT operation