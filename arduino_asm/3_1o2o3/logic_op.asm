; logic_op.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.equ val1 = 0b11001010
.equ val2 = 0b11000101

; making pins output
ldi r16, 0b00111100
out DDRD, r16

; logic operation
ldi r16, val1
ldi r17, val2
and r16, r17
; or r16, r17
; eor r16, r17

; left shift
ldi r17, 0x02
loop_shift:
lsl r16
dec r17
brne loop_shift
out PORTD, r16

start:
rjmp start