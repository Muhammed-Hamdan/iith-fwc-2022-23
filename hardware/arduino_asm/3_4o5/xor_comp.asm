; xor_comp.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.equ value = 0x67
.equ bit_no = 5

; reset system state
ldi r16, 0x00
out SREG, r16
; init stack pointer
ldi r16, high(RAMEND)
out SPH, r16
ldi r16,low(RAMEND)
out SPL, r16
; init pins
ldi r16, 0b00111100
out DDRD, r16
; perform complement
ldi r16, value
ldi r17, bit_no
rcall complement_bit
; output value
ldi r17, 0x02
loop_shift:
lsl r16
dec r17
brne loop_shift
out PORTD, r16

start:
rjmp start

; arguements r16 -> value, r17 -> bit_no
complement_bit:
push r18
ldi r18, 0x01
make_mask:
cpi r17, 0
breq xor_op
lsl r18
dec r17
rjmp make_mask 
xor_op:
eor r16, r17
pop r18
ret
