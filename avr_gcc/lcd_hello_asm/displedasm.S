;Latching given nibble into LCD controller
#define __SFR_OFFSET 0
#define en_pin 7
#include <avr/io.h>

.global LCD_Latch_Nibble

.section .text

LCD_Latch_Nibble:
	PUSH R24
	PUSH R20
	LDI R20, 2
	RCALL LOOPLSL
	IN R20, PORTD
	ANDI R20, 0b11000011
	OR R20, R24
	OUT PORTD, R20
	RCALL PULSE_EN
	POP R20
	POP R24
	RET

;loop for bit shift to left
LOOPLSL:	
	LSL R24			;left shift
	DEC R20			;counter --
	BRNE LOOPLSL	;if counter != 0
	RET

PULSE_EN:
	SBI PORTD, en_pin		; Set en_pin
	RCALL DELAY_EN
	CBI PORTD, en_pin		; Clear en_pin
	RCALL DELAY_EN
	RET

DELAY_EN:
	ldi r24, 0x80
	delay_en_loop1:
	ldi r20, 0x00
	delay_en_loop2:
	dec r20
	cpi r20, 0x00
	brne delay_en_loop2
	dec r24
	cpi r24, 0x00
	brne delay_en_loop1
	ret
