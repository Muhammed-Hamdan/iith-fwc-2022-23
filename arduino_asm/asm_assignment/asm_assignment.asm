; pulse width modulator
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def shift_reg = r5
.def temp0 = r9
.def input = r10
.def result = r21
.def shift_no = r22

.def p = r11
.def q = r12
.def r = r13
.def s = r14
.def t = r15

.def a = r16
.def b = r17
.def c = r18
.def d = r19
.def e = r20

.def a_bar = r0
.def b_bar = r1
.def c_bar = r2
.def d_bar = r3
.def e_bar = r4

.equ a_bit = 4
.equ b_bit = 3
.equ c_bit = 2
.equ d_bit = 1
.equ e_bit = 0

.equ clk_bit = 5


;Initialize system status
ldi r16, 0
out SREG, r16
;Initialize SP
ldi r16, high(RAMEND)
out SPH, r16
ldi r16, low(RAMEND)
out SPL, r16
rcall gpio_timer_init
rcall fsm_init          ; Enter fsm in known state (0)

; *********************************************** ;
MAIN:
rcall fsm_state_update
rcall fsm_output_update
rcall clock_tock
rcall clock_tick
rcall read_input
rjmp MAIN
; *********************************************** ;

gpio_timer_init:
;d2,d3,d4,d5,d6,d13(clk) as output, d8,d9,d10,d11,d12 as input
ldi r16, 0b01111100
out DDRD, r16
ldi r16, 0b00100000 ; input => pin0-4, clk => pin5
out DDRB, r16
;Set prescaler to 1024 
ldi r16, 0b00000101
out TCCR0B, r16
ret

fsm_init:
clr input
ret

read_input:
in input, PINB
ret

left_shift:
lsl shift_reg
dec shift_no
brne left_shift
ret

right_shift:
lsr shift_reg
dec shift_no
brne right_shift
ret

fsm_state_update:
; clear registers
clr a
clr b
clr c
clr d
clr e

; split into boolean variables (1 => 0xff, 0 => 0x00)
sbrc input, a_bit
com a
mov a_bar, a
com a_bar

sbrc input, b_bit
com b
mov b_bar, b
com b_bar

sbrc input, c_bit
com c
mov c_bar, c
com c_bar

sbrc input, d_bit
com d
mov d_bar, d
com d_bar

sbrc input, e_bit
com e
mov e_bar, e
com e_bar

;perform logic
;for t
mov t, c
and t, d_bar
mov temp0, b
and temp0, d
or t, temp0
mov temp0, a
and temp0, d_bar
or t, temp0
mov temp0, a
and temp0, e
or t, temp0
mov temp0, a_bar
and temp0, c_bar
and temp0, d
or t, temp0

;for s
mov s, a_bar
and s, b_bar
and s, e
mov temp0, b_bar
and temp0, d_bar
and temp0, e
or s, temp0
mov temp0, b
and temp0, d_bar
and temp0, e_bar
or s, temp0
mov temp0, b
and temp0, c
and temp0, e_bar
or s, temp0
mov temp0, a_bar
and temp0, c_bar
and temp0, d
and temp0, e
or s, temp0

;for r
mov r, b_bar
and r, d
and r, e
mov temp0, c_bar
and temp0, d
and temp0, e
or r, temp0
mov temp0, a
and temp0, d_bar
and temp0, e_bar
or r, temp0
mov temp0, a_bar
and temp0, b_bar
and temp0, c_bar
and temp0, e
or r, temp0
mov temp0, b_bar
and temp0, c
and temp0, d_bar
and temp0, e_bar
or r, temp0

;for q
mov q, b_bar
and q, c
and q, d_bar
mov temp0, b_bar
and temp0, c
and temp0, e
or q, temp0
mov temp0, a_bar
and temp0, b_bar
and temp0, c_bar
and temp0, e_bar
or q, temp0
mov temp0, b
and temp0, c_bar
and temp0, d
and temp0, e
or q, temp0

;for p
mov p, b
and p, d_bar
mov temp0, b
and temp0, c_bar
and temp0, e_bar
or p, temp0
mov temp0, b_bar
and temp0, c
and temp0, d
and temp0, e_bar
or p, temp0

ret

fsm_output_update:
; clear result
clr result

; construct result
sbrc p, 0
sbr result, 0x01
lsl result
sbrc q, 0
sbr result, 0x01
lsl result
sbrc r, 0
sbr result, 0x01
lsl result
sbrc s, 0
sbr result, 0x01
lsl result
sbrc t, 0
sbr result, 0x01

; output result
mov shift_reg, result
ldi shift_no, 2
rcall left_shift
mov result, shift_reg
out PORTD, result
ret

clock_tick:
sbi PORTB, clk_bit
rcall DELAY
ret

clock_tock:
cbi PORTB, clk_bit
rcall DELAY
ret

; Delay routine
DELAY:
ldi r19, 0x40   ; actual use 0x40
DELAY_loop:
in r16, TIFR0
andi r16, 0x01  ; Check if timer overflow (given 0x02 which mask for compare)
breq DELAY_loop ; If not, keep checking
ldi r16, 0x01   ; Clear flag to check again
out TIFR0, r16
dec r19
brne DELAY_loop
ret
