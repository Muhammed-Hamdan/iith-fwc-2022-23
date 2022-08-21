;enable pin 13 of Arduino as output
#define __SFR_OFFSET 0
#include <avr/io.h>

.global gpio_init

.section .text

gpio_init:
	LDI R16,0b11111100
	OUT DDRD,R16
	LDI R16,0b00000001
	OUT DDRB,R16
	RET

