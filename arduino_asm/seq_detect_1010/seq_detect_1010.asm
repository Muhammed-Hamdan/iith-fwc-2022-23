;seq_detect_1010.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def a = r0
.def b = r1
.def c = r2
.def d = r3
.def e = r9

.def v = r4
.def w = r5
.def x = r6
.def y = r7
.def z = r8

.def result = r17
.def input = r18
.def shift_reg = r19
.def shift_no = r20
.def temp0 = r21
.def temp1 = r22

.equ v_bit = 0
.equ w_bit = 1
.equ x_bit = 2
.equ y_bit = 3
.equ z_bit = 4
.equ clk_bit = 5
.equ input_bit = 2
.equ led_bit = 3


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
;d2,d3,d4,d5 as input, d6,d7,d8,d9,d13(clk) as output
ldi r16, 0b00111100
out DDRD, r16
ldi r16, 0b00101000 ; input => pin2, out_led => pin3, clk => pin5
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
lsl input
sbrc input, 2+3 ; 2 is input pin + 2 left shift done
sbr input, 0x01 
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
clr v
clr w
clr x
clr y
clr z

; split into boolean variables (1 => 0xff, 0 => 0x00)
sbrc input, v_bit
com v
sbrc input, w_bit
com w
sbrc input, x_bit
com x
sbrc input, y_bit
com y
sbrc input, z_bit
com z

; perform logic
; for e
mov e, y
mov temp0, v
com temp0 
and e, temp0

; for d
mov d, y
mov temp0, v
com temp0 
and d, temp0

; for c (commented non-overlap)
; mov c, z
; com c
mov c, x
and c, v

; for b
mov b, w
mov temp0, v
com temp0
and b, temp0

; for a
mov a, w
com a
and a, v

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
; led o/p
sbi PORTB, led_bit
sbrc e, 0
cbi PORTB, led_bit
; D-ff o/p
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