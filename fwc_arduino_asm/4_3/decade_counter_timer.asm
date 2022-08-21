;logic_xor.asm
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
;d2,d3,d4,d5 as output
ldi r16, 0b00111100
out DDRD, r16
;Set prescaler to 1024 
ldi r16, 0b00000101
out TCCR0B, r16

; Locate number 0-9 in 0x0100 to 0x0109
ldi xl, 0x00
ldi xh, 0x01
ldi r16, 0x00
ldi r17, 0x0a
loop_cnt:
st x, r16
inc r16
inc xl
dec r17
brne loop_cnt

start:
ldi r17, 0x0a
clr xl
loop_inc:
ldi r16, 0b00000010
ld r0, x
loopw:  ; left shift #r16 times
lsl r0
dec r16
brne loopw
out PORTD, r0
call DELAY
inc xl
dec r17
brne loop_inc
rjmp start

; Delay routine
DELAY:
ldi r19, 0x08
DELAY_loop:
in r16, TIFR0
andi r16, 0x01  ; Check if timer overflow (given 0x02 which mask for compare)
breq DELAY_loop ; If not, keep checking
ldi r16, 0x01   ; Clear flag to check again
out TIFR0, r16
dec r19
brne DELAY_loop
ret