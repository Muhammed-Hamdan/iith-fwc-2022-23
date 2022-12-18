#include<Arduino.h>
#define PIN_START 2
#define PIN_END 8
void sevenseg(const char code)
{
  unsigned char mask = 0x40;
  for(int i=PIN_START; i<=PIN_END; i++){
  		digitalWrite(i, (code&mask)?(int)1:(int)0);
		mask>>=1;
	}
}
void setup()
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
}
char number_codes[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x73};
void loop()
{
	for(int i=0; i<10; i++){
		sevenseg(~number_codes[i]);
		delay(1000);	
	}
}
