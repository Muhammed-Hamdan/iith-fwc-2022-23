// Decade counter -> 0-9 on seven segment display
#include <avr/io.h>
#include <util/delay.h>

void gpio_init(void){
  //set PD2-PD7 as output pins 0xFC=0b11111100 (binary)
  DDRD   |= 0xFC;
  //set PB0 as output pin
  DDRB    |= ((1 << DDB0));
}

void seven_seg_display(char code){
  char portb_mask, portd_mask;
  portb_mask = (code & 0x40) >> 6;
  portd_mask = (code & 0x3f) << 2;
  PORTB &= ~(0x01);
  PORTB |= portb_mask;
  PORTD &= ~(0xFC);
  PORTD |= portd_mask; 
}
 
int main (void)
{
	char sevseg_code_arr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
  int count;

  gpio_init();
  while (1) {
    for(count = 0; count < 10; count++){
      seven_seg_display(~sevseg_code_arr[count]);
      _delay_ms(100);
    }
  }

  /* . */
  return 0;

}
