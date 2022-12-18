#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h>
#include <ctype.h>

/****************** UART functions ******************/

#define UART_BAUD  9600l
#define _FDEV_EOF  -1
#define _FDEV_ERR  -2

void
uart_init(void)
{
#if F_CPU < 2000000UL && defined(U2X)	// F_CPU in Makefile
  UCSR0A = _BV(U2X);             /* improve baud rate error by using 2x clk */
  UBRR0L = (F_CPU / (8UL * UART_BAUD)) - 1; 
#else
  UBRR0L = (F_CPU / (16UL * UART_BAUD)) - 1;
#endif
  UCSR0B = _BV(TXEN0) | _BV(RXEN0); /* tx/rx enable */
}

int
uart_getchar(void)
{
	uint8_t c;
	
	loop_until_bit_is_set(UCSR0A, RXC0);
	if (UCSR0A & _BV(FE0))
		return _FDEV_EOF;
	if (UCSR0A & _BV(DOR0))
		return _FDEV_ERR;
	c = UDR0;
	return c;
}

/****************** LCD functions *********************/

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
 _delay_us(40); 	  // wait 40 microseconds
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
 SendNibble(data); 		// send upper 4 bits
 SendNibble(data<<4); 	// send lower 4 bits
}
void LCD_Cmd (byte cmd)
{
 ClearBit(PORTD,LCD_RS); // R/S line 0 = command data
 SendByte(cmd); 		 // send it
}
void LCD_Char (byte ch)
{
 SetBit(PORTD,LCD_RS); // R/S line 1 = character data
 SendByte(ch); 		   // send it
}
void LCD_Init(void)
{
 LCD_Cmd(0x33); // initialize controller
 LCD_Cmd(0x32); // set to 4-bit input mode
 LCD_Cmd(0x28); // 2 line, 5x7 matrix
 LCD_Cmd(0x0E); // turn cursor on (0x0C to off)
 LCD_Cmd(0x06); // cursor direction = right
 LCD_Cmd(0x01); // start with clear display
 LCD_Cmd(0x80);	// Move to first line 
 _delay_ms(3);  // wait for LCD to initialize
}
void LCD_Clear(void) // clear the LCD display
{
 LCD_Cmd(CLEARDISPLAY);
 LCD_Cmd(CURSOR_LINE1);
 _delay_ms(3); 		// wait for LCD to process command
}

void LCD_Message(const char *text) // display string on LCD
{
 while (*text) 					  // do until /0 character
 LCD_Char(*text++); 			  // send char & update char pointer
}

void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
 char st[8] = ""; 	// save enough space for result
 itoa(data,st,10);  // 
 LCD_Message(st); 	// display in on LCD
}


/************ Calculator functions *****************/

#define CALC_BUFSIZE 17
#define CALCULATE_KEY '\r'
#define BACK_SPACE ','
#define CLEAR_SCREEN '.'
#define OP_ERROR	-1
#define PARSE_ERROR	-1
#define CALC_ERROR	-1
#define CALC_SUCCESS 0


void gpio_init(void){
    // use PortB for LCD interface
    DDRD = 0xFE; // 1111.1110; set PD7-PD2 as outputs
	DDRB = 0x20;
}

int read_input_locking(uint8_t buffer[]){
	int counter = 0, ch;
	ch = uart_getchar();
	while(ch != CALCULATE_KEY){
		if(ch == BACK_SPACE){
			counter = (counter==0)?0:counter-1;
			LCD_Cmd(0x80|(uint8_t)counter);
			LCD_Char(' ');
			LCD_Cmd(0x80|(uint8_t)counter);
		}
		else if(ch == CLEAR_SCREEN){
			LCD_Clear();
			counter = 0;
		}
		else {
			buffer[counter] = ch;
			if(counter < CALC_BUFSIZE-1) counter++;
			LCD_Char((uint8_t)ch);
		}
		ch = uart_getchar();
	}
	buffer[counter] = '\0';
	return counter;
}

long long custom_atoi(uint8_t input[], int sign){
	int i = 0;
	long long result = 0;
	while(input[i]!='\0') result = 10*result + (long long) input[i++] - (long long)'0';
	if(sign == '-') result = -result;
	return result;
}

int custom_itoa(long long num, uint8_t buffer[], int end){
	int counter = end, sign = '+';
	buffer[end] = '\0';
	if(num == 0) buffer[--counter] = '0';
	if(num < 0){
		sign = '-';
		num = -num;
	}
	while(num != 0){
		buffer[--counter] = num%10 + (long long)'0';
		num /= 10;
	}
	if(sign == '-') buffer[--counter] = '-';
	return counter;
}

int find_operation(uint8_t character){
	if(character == '+' || character == '-' || character == '*' || character == '/' || character == '%') return (int)character;
	else return OP_ERROR;
}

int parse_input(uint8_t buffer[], long long *a, long long *b){
	int counter = 0, operation, position = 0, signa = '+', signb = '+';
	if(buffer[counter]=='-'){
		signa = '-';
		counter++;
		position = 1;
	}
	while(isdigit(buffer[counter])) counter++;
	operation = find_operation(buffer[counter]);
	if(operation == OP_ERROR) return PARSE_ERROR;
	buffer[counter++] = '\0';
	*a = custom_atoi(&buffer[position], signa);
	if(buffer[counter]=='-'){
		signb = '-';
		counter++;
	}
	position = counter;
	while(isdigit(buffer[counter])) counter++;
	if(buffer[counter]!='\0' || buffer[position]=='\0') return PARSE_ERROR;
	*b = custom_atoi(&buffer[position], signb);
	return operation;
}

long long calculate(long long *a, long long *b, long long *result, int operation){
	switch(operation){
		case '+':
			*result = *a + *b;
			break;
		case '-':
			*result = *a - *b;
			break;
		case '*':
			*result = *a * *b;
			break;
		case '/':
			if(*b == 0) return CALC_ERROR; 
			*result = *a / *b;
			break;
		case '%':
			*result = *a % *b;
			break;
		default:
			return CALC_ERROR;
	}
	return CALC_SUCCESS;
}

void print_result(char output[]){
	LCD_Cmd(CURSOR_LINE2);
	LCD_Message(output);
}

/*********************** MAIN PROGRAM *********************/
int main(void)
{
    uint8_t calc_buffer[CALC_BUFSIZE];
	int parse_code, calc_code, position;
	long long a,b, result;
	gpio_init();
	LCD_Init();
	uart_init();
	while(1){
		
		read_input_locking(calc_buffer);
		parse_code = parse_input(calc_buffer, &a, &b);
		if(parse_code == PARSE_ERROR){
			print_result("Invalid_input!");
			continue;
		}
		calc_code = calculate(&a, &b, &result, parse_code);
		if(calc_code == CALC_ERROR){
			print_result("Div_by_zero!");
			continue;
		}
		position = custom_itoa(result, calc_buffer, CALC_BUFSIZE-1);
		print_result((char *)&calc_buffer[position]);

	}
}
