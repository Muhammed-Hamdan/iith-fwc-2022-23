;blink_timer.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

;Initialize system status
ldi r16, 0
out SREG, r16
;Initialize SP
ldi r16, high(RAMEND)
out SPH, r16
ldi r16, low(RAMEND)
out SPL, r16
;d9/PB0 as output
ldi r16, 0b00000001
out DDRB, r16
;Set prescaler to 1024 
ldi r16, 0b00000101
out TCCR0B, r16
clr r18
; ldi xl, 0x00
; ldi xh, 0x01
; ldi r16, 0b00000001
; st x, r16

loop:
ldi r16, 0b00000001 ; Given 0b00000010 which mask for pin_10
eor r18, r16
out PORTB, r18
ldi r19, 0x08   ; Actual given 0x40; for double delay load 0x80
rcall PAUSE
rjmp loop

PAUSE:
PAUSE_loop:
in r16, TIFR0
andi r16, 0x01  ; Check if timer overflow (given 0x02 which mask for compare)
breq PAUSE_loop ; If not, keep checking
ldi r16, 0x01   ; Clear flag to check again
out TIFR0, r16
dec r19
brne PAUSE_loop
ret
