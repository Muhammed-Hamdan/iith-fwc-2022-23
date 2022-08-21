;lcd_hello.asm
.nolist
.include "/usr/share/avra/m328Pdef.inc"
.list

.def lcd_data_reg = r19

.equ cmd_initctrlr = 0x33
.equ cmd_4bitinput = 0x32
.equ cmd_5x7_2line = 0x28
.equ cmd_entrymode = 0x06
.equ cmd_displayon = 0x0E
.equ cmd_cleardisplay = 0x01
.equ cmd_firstline = 0x80


.equ en_pin = 7
.equ rs_pin = 6

;Initialize system status
ldi r16, 0
out SREG, r16
;Initialize SP
ldi r16, high(RAMEND)
out SPH, r16
ldi r16, low(RAMEND)
out SPL, r16
rcall gpio_init
rcall lcd_init
ldi zl, low(2*hello_str)
ldi zh, high(2*hello_str)
rcall lcd_print_str
end:
rjmp end

gpio_init:
; d2, d3, d4, d5, d6, d7 output
ldi r16, 0b11111100
out DDRD, r16
ret

lcd_init:
ldi lcd_data_reg, cmd_initctrlr
rcall lcd_write_cmd
ldi lcd_data_reg, cmd_4bitinput
rcall lcd_write_cmd
ldi lcd_data_reg, cmd_5x7_2line
rcall lcd_write_cmd
ldi lcd_data_reg, cmd_entrymode
rcall lcd_write_cmd
ldi lcd_data_reg, cmd_displayon
rcall lcd_write_cmd
ldi lcd_data_reg, cmd_cleardisplay
rcall lcd_write_cmd
ldi lcd_data_reg, cmd_firstline
rcall lcd_write_cmd
ret

lcd_print_str:
lcd_print_str_loop:
lpm lcd_data_reg, z+
cpi lcd_data_reg, 0
breq lcd_print_str_return
rcall lcd_write_data
rjmp lcd_print_str_loop
lcd_print_str_return:
ret

lcd_write_cmd:
cbi PORTD, rs_pin
rcall lcd_latch_data
ret

lcd_write_data:
sbi PORTD, rs_pin
rcall lcd_latch_data
ret

lcd_latch_data:
mov r17, lcd_data_reg
andi r17, 0xf0
lsr r17
lsr r17
in r18, PORTD
andi r18, ~(0b00111100)
or r18, r17
out PORTD, r18
rcall pulse_en
mov r17, lcd_data_reg
andi r17, 0x0f
lsl r17
lsl r17
in r18, PORTD
andi r18, ~(0b00111100)
or r18, r17
out PORTD, r18
rcall pulse_en
ret

pulse_en:
sbi PORTD, en_pin
rcall delay_en
cbi PORTD, en_pin
rcall delay_en
ret

delay_en:
ldi r20, 0x80
delay_en_loop1:
ldi r16, 0x00
delay_en_loop2:
dec r16
cpi r16, 0x00
brne delay_en_loop2
dec r20
cpi r20, 0x00
brne delay_en_loop1
ret

hello_str:
.db "Hello world!",0x00