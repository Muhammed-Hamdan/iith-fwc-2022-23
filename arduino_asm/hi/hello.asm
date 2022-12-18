;hello
;using assembly language for turning LED on

.nolist ;Turn of listing
.include "/usr/share/avra/m328Pdef.inc"
.list   ;Turn on listing

  ldi r16,0b00100000
  out DDRB,r16
  ldi r17,0b00100000 ;For LED-off use 0b00000000
  out PortB,r17
Start:
  rjmp Start
