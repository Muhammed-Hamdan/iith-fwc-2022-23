;decade_counter_fsm.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def a = r0
.def b = r1
.def c = r2
.def d = r3

.def w = r4
.def x = r5
.def y = r6
.def z = r7

.def result = r17
.def input = r18
.def shift_reg = r19
.def shift_no = r20
.def temp0 = r21
.def temp1 = r22

.equ w_bit = 0
.equ x_bit = 1
.equ y_bit = 2
.equ z_bit = 3
.equ clk_bit = 2


;Initialize system status
ldi r16, 0
out SREG, r16
;Initialize SP
ldi r16, high(RAMEND)
out SPH, r16
ldi r16, low(RAMEND)
out SPL, r16
rcall gpio_timer_init
rcall fsm_init

; *********************************************** ;
MAIN:
rcall fsm_state_update
rcall fsm_output_update
rcall clock_tick
rcall clock_tock
rcall read_input
rjmp MAIN
; *********************************************** ;

gpio_timer_init:
;d2,d3,d4,d5 as input, d6,d7,d8,d9,d10(clk) as output
ldi r16, 0b00111100
out DDRD, r16
ldi r16, 0b00000100
out DDRB, r16
;Set prescaler to 1024 
ldi r16, 0b00000101
out TCCR0B, r16
ret

fsm_init:
clr input
ret

read_input:
in shift_reg, PINB
ldi shift_no, 2
call left_shift
mov input, shift_reg
in shift_reg, PIND
ldi shift_no, 6
call right_shift
or input, shift_reg
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
clr w
clr x
clr y
clr z

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
ret

fsm_output_update:
; clear result
clr result

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
