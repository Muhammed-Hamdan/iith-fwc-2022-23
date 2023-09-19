; 7447_logic.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def A = r4
.def B = r5
.def C = r6
.def D = r7
.def Abar = r8
.def Bbar = r9
.def Cbar = r10
.def Dbar = r11
.def input = r12
.def temp0 = r13
.def result = r17

.def l = r0
.def m = r1
.def n = r2
.def o = r3
.def p = r14
.def q = r15
.def r = r16

.equ A_bit = 1
.equ B_bit = 2
.equ C_bit = 3
.equ D_bit = 4

; reset system state
ldi r16, 0x00
out SREG, r16
; init stack pointer
ldi r16, high(RAMEND)
out SPH, r16
ldi r16,low(RAMEND)
out SPL, r16
; init pins (PB1-PB4:INPUT, PD2-PD7,PB0: OUTPUT)
ldi r16, 0b11111100
out DDRD, r16
ldi r16, 0b00000001
out DDRB, r16
main:
; clear registers
clr A
clr B
clr C
clr D
clr result
; read input
in input, PINB
; split into boolean variables (1 => 0xff, 0 => 0x00)
sbrc input, A_bit
com A
mov Abar, A
com Abar
sbrc input, B_bit
com B
mov Bbar, B
com Bbar
sbrc input, C_bit
com C
mov Cbar, C
com Cbar
sbrc input, D_bit
com D
mov Dbar, D
com Dbar
; perform logic
; for a
mov temp0, Bbar
and temp0, Dbar
mov l, temp0
mov temp0, Abar
and temp0, C
or l, temp0
mov temp0, B
and temp0, C
or l, temp0
mov temp0, A
and temp0, Dbar
or l, temp0
mov temp0, Abar
and temp0, B
and temp0, D
or l, temp0
mov temp0, A
and temp0, Bbar
and temp0, Cbar
or l, temp0
com l

;for b
mov temp0, Abar
and temp0, Bbar
mov m, temp0
mov temp0, Bbar
and temp0, Dbar
or m, temp0
mov temp0, Abar
and temp0, Cbar
and temp0, Dbar
or m, temp0
mov temp0, Abar
and temp0, C
and temp0, D
or m, temp0
mov temp0, A
and temp0, Cbar
and temp0, D
or m, temp0
com m

; for c
mov temp0, Abar
and temp0, Cbar
mov n, temp0
mov temp0, Abar
and temp0, D
or n, temp0
mov temp0, Cbar
and temp0, D
or n, temp0
mov temp0, Abar
and temp0, B
or n, temp0
mov temp0, A
and temp0, Bbar
or n, temp0
com n

; for d
mov temp0, A
and temp0, Cbar
mov o, temp0
mov temp0, Abar
and temp0, Bbar
and temp0, Dbar
or o, temp0
mov temp0, Bbar
and temp0, C
and temp0, D
or o, temp0
mov temp0, B
and temp0, Cbar
and temp0, D
or o, temp0
mov temp0, B
and temp0, C
and temp0, Dbar
or o, temp0
com o

; for e
mov temp0, Bbar
and temp0, Dbar
mov p, temp0
mov temp0, C
and temp0, Dbar
or p, temp0
mov temp0, A
and temp0, C
or p, temp0
mov temp0, A
and temp0, B
or p, temp0
com p

; for f
mov temp0, Cbar
and temp0, Dbar
mov q, temp0
mov temp0, B
and temp0, Dbar
or q, temp0
mov temp0, A
and temp0, Bbar
or q, temp0
mov temp0, A
and temp0, C
or q, temp0
mov temp0, Abar
and temp0, B
and temp0, Cbar
or q, temp0
com q

; for g
mov temp0, Bbar
and temp0, C
mov r, temp0
mov temp0, C
and temp0, Dbar
or r, temp0
mov temp0, A
and temp0, Bbar
or r, temp0
mov temp0, A
and temp0, D
or r, temp0
mov temp0, Abar
and temp0, B
and temp0, Cbar
or r, temp0
com r

; construct result
sbrc r, 0
sbr result, 0x01
lsl result
sbrc q, 0
sbr result, 0x01
lsl result
sbrc p, 0
sbr result, 0x01
lsl result
sbrc o, 0
sbr result, 0x01
lsl result
sbrc n, 0
sbr result, 0x01
lsl result
sbrc m, 0
sbr result, 0x01
lsl result
sbrc l, 0
sbr result, 0x01
lsl result
rol result
; output result
brcs set_g
cbi PORTB, 0
rjmp out_result
set_g:
sbi PORTB, 0
out_result:
out PORTD, result

rjmp main