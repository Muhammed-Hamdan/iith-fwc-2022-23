;decoder.asm
;driving the display decoder

.include "/usr/share/avra/m328Pdef.inc"

ldi r16, 0b00111100
out DDRD, r16

ldi r16, 0b00010100
out PORTD, r16

Start:
rjmp Start
