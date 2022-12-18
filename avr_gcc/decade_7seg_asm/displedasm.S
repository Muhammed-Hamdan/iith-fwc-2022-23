;turning led on/off
#define __SFR_OFFSET 0
#include <avr/io.h>

.global display_7seg

.section .text

display_7seg:
	PUSH R24
	PUSH R16
	PUSH R20
	CBI PORTB, 0
	SBRC R24, 6
	SBI PORTB, 0
	LDI R20, 2
	RCALL looplsl
	OUT PORTD, R24
	POP R20
	POP R16
	POP R24
	RET

;loop for bit shift to left
looplsl:	
	LSL R24			;left shift
	DEC r20			;counter --
	BRNE	looplsl	;if counter != 0
	RET
