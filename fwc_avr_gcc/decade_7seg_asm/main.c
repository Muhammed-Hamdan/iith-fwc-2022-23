//Blink LED
//through an assembly routine
#include <avr/io.h>

//Function declared in initasm.S
extern void gpio_init(void);
//Function declared in displedasm.S
extern void display_7seg(uint8_t);
//Function declared in delayasm.S
extern void delay(uint8_t, uint8_t, uint8_t);

 int main (void)
{
	uint8_t sevseg_code_arr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	int count;
	gpio_init();
	while (1) {
		for(count = 0; count<10; count++){
			display_7seg(~sevseg_code_arr[count]);
			delay(0, 0, 10);
		}	  	  
	}
	return 0;

}
