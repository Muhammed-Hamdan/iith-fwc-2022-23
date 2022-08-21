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
call wait
inc xl
dec r17
brne loop_inc
rjmp start

; Delay routine
wait:
push r16
push r17
push r18

ldi r16, 0x08
ldi r17, 0x00
ldi r18, 0x00

w0:
dec r18
brne w0
dec r17
brne w0
dec r16
brne w0

pop r18
pop r17
pop r16
ret