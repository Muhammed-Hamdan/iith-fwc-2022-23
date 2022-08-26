//Turns LED on and off
#include <avr/io.h>
#include <util/delay.h>

#define CLK_BIT 5

// abcde => inputs => PB4-PB0; pqrst => outputs => PD6-PD2

int A, B, C, D, E, P, Q, R, S, T;

void gpio_init(void){
  DDRB = 0b00100000;
  DDRD = 0b01111100;
}

void fsm_read(void){
  uint8_t input = PORTB;
  E = input&0x01;
  input>>=1;
  D = input&0x01;
  input>>=1;
  C = input&0x01;
  input>>=1;
  B = input&0x01;
  input>>=1;
  A = input&0x01;
}

void fsm_update(void){
  uint8_t output;

  P = (B&&!D) || (B&&!C&&!E) || (!B&&C&&D&&!E);
  Q = (!B&&C&&!D) || (!B&&C&&E) || (!A&&!B&&!C&&!E) || (B&&!C&&D&&E);
  R = (!B&&D&&E) || (!C&&D&&E) || (A&&!D&&!E) || (!A&&!B&&!C&&E) || (!B&&C&&!D&&!E);
  S = (!A&&!B&&E) || (!B&&!D&&E) || (B&&!D&&!E) || (B&&C&&!E) || (!A&&!C&&D&&E);
  T = (C&&!D) || (B&&D) || (A&&!D) || (A&&E) || (!A&&!C&&D);

  output = (P<<4) | (Q<<3) | (R<<2) | (S<<1) | T;
  PORTD = output<<2;
}

void clock_pulse(void){
  PORTB &= ~(1<<CLK_BIT);
  _delay_ms(2000);
  PORTB |= (1<<CLK_BIT);
  _delay_ms(2000);
}

int main (void)
{
	gpio_init();
  while(1){
    fsm_read();
    fsm_update();
    clock_pulse();
  }
  return 0;
}
