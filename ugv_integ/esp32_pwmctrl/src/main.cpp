#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include "esp32_eoss3_spi.h"

#define MISC_BASE_ADDR	0x40005000
#define SW_MB_1		MISC_BASE_ADDR+0x110
#define SW_MB_2		MISC_BASE_ADDR+0x114

#define SW_MB_1_DEF_VAL	0
#define SW_MB_2_DEF_VAL	0

uint32_t dest_address = SW_MB_1_DEF_VAL;

void _setup() {
  Dabble.begin("Esp32");
}

void _loop() {
  Dabble.processInput(); //Continously process input from the dabble app over bluetooth
}

#define MAP_FROM_HIGH	7.0f
#define MAP_FROM_LOW	-7.0f
#define MAP_TO_HIGH	255.0f
#define MAP_TO_LOW	-255.0f

int16_t joystick_pwm_map(float axis_value){
	int16_t pwm_val = (int16_t) (axis_value*(MAP_TO_HIGH-MAP_TO_LOW)/(MAP_FROM_HIGH-MAP_FROM_LOW));
	return pwm_val;
}

void setup(){
	_setup();

	uint8_t* read_buff = (uint8_t*)&dest_address;

	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, 1);

	esp32_eoss3_spi_init();	
	do{
		esp32_eoss3_spi_ahb_read(SW_MB_1, read_buff, 4);
		delay(10);
	}while(dest_address == SW_MB_1_DEF_VAL);

	digitalWrite(LED_BUILTIN, 0);
}

void loop(){
	static uint32_t shape_button_status = 0, previous_status = 0;
	static int16_t pwm_val[2] = {0,0}, previous_val[2] = {0,0};
	uint8_t* write_buff = NULL;
	
	_loop();
	write_buff = (uint8_t*)&shape_button_status;
	write_buff[0] = (uint8_t)GamePad.isCrossPressed();	
	write_buff[1] = (uint8_t)GamePad.isSquarePressed();	
	write_buff[2] = (uint8_t)GamePad.isTrianglePressed();	
	write_buff[3] = (uint8_t)GamePad.isCirclePressed();	
	pwm_val[0] = joystick_pwm_map(GamePad.getXaxisData());
	pwm_val[1] = joystick_pwm_map(GamePad.getYaxisData());
	if(shape_button_status != previous_status || pwm_val[0] != previous_val[0] || pwm_val[1] != previous_val[1]){
		previous_status = shape_button_status;
		previous_val[0] = pwm_val[0];
		previous_val[1] = pwm_val[1];

		Serial.println(pwm_val[0]);
		Serial.println(pwm_val[1]);

		write_buff = (uint8_t*)&shape_button_status;
		esp32_eoss3_spi_ahb_write(dest_address, write_buff, 4);

		write_buff = (uint8_t*)&pwm_val[0];
		esp32_eoss3_spi_ahb_write(dest_address+4, write_buff, 4);
	}
	
	/*
	Serial.println("X-axis:");
	Serial.print(((int8_t)GamePad.getXaxisData())<<5);
	Serial.println("Y-axis:");
	Serial.print(((int8_t)GamePad.getYaxisData())<<5);
	delay(100);
	*/
}

