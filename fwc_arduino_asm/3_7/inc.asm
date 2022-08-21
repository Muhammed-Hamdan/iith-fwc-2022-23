; xor_comp.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def w = r0
.def x = r1
.def y = r2
.def z = r3

.def a = r4
.def b = r5
.def c = r6
.def d = r7

.def input = r9
.def temp0 = r10
.def temp1 = r11
.def temp2 = r12
.def result = r18

.equ w_bit = 0
.equ x_bit = 1
.equ y_bit = 2
.equ z_bit = 3

; reset system state
ldi r16, 0x00
out SREG, r16
; init stack pointer
ldi r16, high(RAMEND)
out SPH, r16
ldi r16,low(RAMEND)
out SPL, r16
; init pins (PB0-PB3:INPUT, PD2-PD5: OUTPUT)
ldi r16, 0b00111100
out DDRD, r16
ldi r16, 0b00000000
out DDRB, r16
main:

; clear registers
clr w
clr x
clr y
clr z
clr a
clr b
clr c
clr d
clr result
; read input
in input, PINB
; split into boolean variables (1 => 0xff, 0 => 0x00)
sbrc input, w_bit
com w
sbrc input, x_bit
com x
sbrc input, y_bit
com y
sbrc input, z_bit
com z
; perform logic
; for a
mov temp0, w
com temp0
mov a, temp0
; for b
mov temp0, w
mov temp1, x
com temp1
and temp0, temp1
mov temp1, z
com temp1
and temp0, temp1
mov b, temp0
mov temp0, w
com temp0
and temp0, x
or b, temp0
; for c
mov temp0, w
and temp0, x
mov temp1, y
com temp1
and temp0, temp1
mov c, temp0
mov temp0, x
com temp0
and temp0, y
or c, temp0
mov temp0, w
com temp0
and temp0, y
or c, temp0
; for d
mov temp0, w
and temp0, x
and temp0, y
mov d, temp0
mov temp0, w
com temp0
and temp0, z
or d, temp0
; construct result
sbrc d, 0
sbr result, 0x01
lsl result
sbrc c, 0
sbr result, 0x01
lsl result
sbrc b, 0
sbr result, 0x01
lsl result
sbrc a, 0
sbr result, 0x01
; output result
ldi r17, 0x02
loop_shift:
lsl result
dec r17
brne loop_shift
out PORTD, result

rjmp main