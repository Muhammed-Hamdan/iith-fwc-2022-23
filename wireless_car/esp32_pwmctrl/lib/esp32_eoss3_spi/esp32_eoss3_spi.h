// esp32_eoss3_spi header

#ifndef _ARDUINO_H_INCLUDED
#define _ARDUINO_H_INCLUDED
#include <Arduino.h>
#endif

#include <SPI.h>

#define EOSS3_VSPI_SS 	27
#define EOSS3_VSPI_MOSI 18
#define EOSS3_VSPI_MISO 19
#define EOSS3_VSPI_CLK 	5

#define EOSS3_SPI_SPEED 8000000

void esp32_eoss3_spi_init(void);
void esp32_eoss3_spi_de_init(void);
void esp32_eoss3_spi_tlc_read(uint8_t address, uint8_t* buffer, uint16_t len);
void esp32_eoss3_spi_tlc_write(uint8_t address, uint8_t* buffer, uint16_t len);
uint8_t esp32_eoss3_spi_tlc_dev_id(void);
int esp32_eoss3_spi_ahb_read(uint32_t m4_address, uint8_t* buffer, uint16_t len);
int esp32_eoss3_spi_ahb_write(uint32_t m4_address, uint8_t* buffer, uint16_t len);
