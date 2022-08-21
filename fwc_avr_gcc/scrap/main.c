//Blink LED
//through an assembly routine
#include <avr/io.h>
#define RS_PIN 6

//Function declared in initasm.S
extern void gpio_init(void);
//Function declared in displedasm.S
extern void LCD_Latch_Nibble(uint8_t);
//Function declared in delayasm.S
extern void delay(uint8_t, uint8_t, uint8_t);

void LCD_Latch_Byte(uint8_t data){
	LCD_Latch_Nibble(data>>4);
	LCD_Latch_Nibble(data&0x0f);
}
void LCD_Cmd(uint8_t cmd){
	PORTD &= ~(1<<6);
	LCD_Latch_Byte(cmd);
}
void LCD_Data(uint8_t data){
	PORTD |= (1<<6);
	LCD_Latch_Byte(data);
}
void LCD_Init(void){
	LCD_Cmd(0x33); // initialize controller
	LCD_Cmd(0x32); // set to 4-bit input mode
	LCD_Cmd(0x28); // 2 line, 5x7 matrix
	LCD_Cmd(0x0C); // turn cursor off (0x0E to enable)
	LCD_Cmd(0x06); // cursor direction = right
	LCD_Cmd(0x01); // start with clear display
}

void LCD_Message(uint8_t text[]){
	while(*text)
	LCD_Data(*text++);
}

 int main (void)
{
	gpio_init();
	LCD_Init();
	LCD_Message("Hello World!");
	while (1) {
		; 	  
	}
	return 0;

}
