#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> 

// TYPEDEFS
typedef uint8_t byte; // changed the name


// ------------------
//LCD DRIVER ROUTINES
//
// Routines:
// LCD_Init initializes the LCD controller
// LCD_Cmd sends LCD controller command
// LCD_Char sends single ascii character to display
// LCD_Clear clears the LCD display & homes cursor
// LCD_Integer displays an integer value
// LCD_Message displays a string
// PortB is used for data communications with the HD44780-controlled LCD.
// The following defines specify which port pins connect to the controller:
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define LCD_RS 6 // pin for LCD R/S     (arduino D6)
#define LCD_E 7 // pin for LCD enable   (arduino D7)
#define DAT4 2 // pin for d4 (arduino D2)
#define DAT5 3 // pin for d5 (arduino D3)
#define DAT6 4 // pin for d6 (arduino D4)
#define DAT7 5 // pin for d7 (arduino D5)
//// The following defines are controller commands
#define CLEARDISPLAY 0x01
#define CURSOR_LINE1 0x80
#define CURSOR_LINE2 0xC0

void PulseEnableLine (void)
{
 SetBit(PORTD,LCD_E); // take LCD enable line high
 _delay_us(40); // wait 40 microseconds
 ClearBit(PORTD,LCD_E); // take LCD enable line low
}
void SendNibble(byte data)
{
 PORTD &= ~0x3C; // 1100.0011 = clear 4 data lines
 if (data & _BV(4)) SetBit(PORTD,DAT4);
 if (data & _BV(5)) SetBit(PORTD,DAT5);
 if (data & _BV(6)) SetBit(PORTD,DAT6);
 if (data & _BV(7)) SetBit(PORTD,DAT7);
 PulseEnableLine(); // clock 4 bits into controller
}
void SendByte (byte data)
{
 SendNibble(data); // send upper 4 bits
 SendNibble(data<<4); // send lower 4 bits
 //ClearBit(PORTD,5); // turn off boarduino LED (use line if PORTB used)
}
void LCD_Cmd (byte cmd)
{
 ClearBit(PORTD,LCD_RS); // R/S line 0 = command data
 SendByte(cmd); // send it
}
void LCD_Char (byte ch)
{
 SetBit(PORTD,LCD_RS); // R/S line 1 = character data
 SendByte(ch); // send it
}
void LCD_Init(void)
{
 LCD_Cmd(0x33); // initialize controller
 LCD_Cmd(0x32); // set to 4-bit input mode
 LCD_Cmd(0x28); // 2 line, 5x7 matrix
 LCD_Cmd(0x0C); // turn cursor off (0x0E to enable)
 LCD_Cmd(0x06); // cursor direction = right
 LCD_Cmd(0x01); // start with clear display
 _delay_ms(3); // wait for LCD to initialize
}
void LCD_Clear(void) // clear the LCD display
{
 LCD_Cmd(CLEARDISPLAY);
 _delay_ms(3); // wait for LCD to process command
}

void LCD_Message(const char *text) // display string on LCD
{
 while (*text) // do until /0 character
 LCD_Char(*text++); // send char & update char pointer
}

void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
 char st[8] = ""; // save enough space for result
 itoa(data,st,10); // 
 LCD_Message(st); // display in on LCD
}

void gpio_init(void){
    // use PortB for LCD interface
    DDRD = 0xFC; // 1111.1100; set PD7-PD2 as outputs
}

// MAIN PROGRAM
int main(void)
{
    char my_str[] = "Decade Counter";
    int count = 0;
    gpio_init();
    LCD_Init(); // initialize LCD controller
    LCD_Message(my_str);
    while(1)
    {
        LCD_Cmd(CURSOR_LINE2);
        LCD_Char(count + '0');
        _delay_ms(100);     // set animation speed
        count = (count == 9)?0:count+1;
    }
}
