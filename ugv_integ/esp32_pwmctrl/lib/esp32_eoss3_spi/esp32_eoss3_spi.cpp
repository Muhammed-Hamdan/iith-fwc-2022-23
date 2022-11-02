// esp32_eoss3_spi source code

#include "esp32_eoss3_spi.h"

#define SPITLC_MEM_ADDR_BYTE_0	0x20
#define SPITLC_MEM_DATA_BYTE_0	0x28
#define SPITLC_AHB_STATUS	0x2F
#define SPITLC_AHB_ACCESS_CTL	0x30
#define SPITLC_SCRATCH_BYTE	0x31
#define SPITLC_S3_GEN_STAT	0x32
#define SPITLC_DMA_ADD0		0x38
#define SPITLC_DMA_STATUS	0x3f
#define SPITLC_DMA_READ_DATA	0x40

#define MAX_WAIT		10

SPIClass* eoss3_spi = NULL;

void esp32_eoss3_spi_init(void){
	eoss3_spi = new SPIClass(VSPI);
	eoss3_spi->begin(EOSS3_VSPI_CLK, EOSS3_VSPI_MISO, EOSS3_VSPI_MOSI, EOSS3_VSPI_SS);
	pinMode(EOSS3_VSPI_SS, OUTPUT);
}

void esp32_eoss3_spi_de_init(void){
	eoss3_spi->end();
}

void esp32_eoss3_spi_tlc_read(uint8_t address, uint8_t* buffer, uint16_t len){
	address &= ~0x80;	// R'/W bit (MSB) is 0
	eoss3_spi->beginTransaction(SPISettings(EOSS3_SPI_SPEED, SPI_MSBFIRST, SPI_MODE0));
	digitalWrite(EOSS3_VSPI_SS, 0);
	eoss3_spi->transfer(address);
	eoss3_spi->transfer16((uint16_t)0x00);	// two dummy bytes 
	eoss3_spi->transfer(buffer, len);
	digitalWrite(EOSS3_VSPI_SS, 1);
	eoss3_spi->endTransaction();
}

void esp32_eoss3_spi_tlc_write(uint8_t address, uint8_t* buffer, uint16_t len){
	address |= 0x80;	// R'W bit (MSB) is 1
	eoss3_spi->beginTransaction(SPISettings(EOSS3_SPI_SPEED, MSBFIRST, SPI_MODE0));
	digitalWrite(EOSS3_VSPI_SS, 0);
	eoss3_spi->transfer(address);
	eoss3_spi->transfer(buffer, len);
	digitalWrite(EOSS3_VSPI_SS, 1);

	// Dummy read of scratch register to generate extra clocks for write
	digitalWrite(EOSS3_VSPI_SS, 0);
	eoss3_spi->transfer(SPITLC_SCRATCH_BYTE);
	eoss3_spi->transfer16((uint16_t)0x00);	// two dummy bytes 
	eoss3_spi->transfer((uint8_t)0x00);
	digitalWrite(EOSS3_VSPI_SS, 1);

	eoss3_spi->endTransaction();
}

uint8_t esp32_eoss3_spi_tlc_dev_id(void){
	uint8_t address;
	eoss3_spi->beginTransaction(SPISettings(EOSS3_SPI_SPEED, MSBFIRST, SPI_MODE0));
	digitalWrite(EOSS3_VSPI_SS, 0);
	eoss3_spi->transfer(0xff);
	eoss3_spi->transfer((uint8_t)0x00);	// dummy byte
	address = eoss3_spi->transfer((uint8_t)0x00);
	digitalWrite(EOSS3_VSPI_SS, 1);
	eoss3_spi->endTransaction();
	return address;
}

int esp32_eoss3_spi_ahb_read(uint32_t m4_address, uint8_t* buffer, uint16_t len){
	uint8_t dma_data[4+2], dmaStatus;
	uint32_t counter = 0;

	uint16_t dmaReadCount = len - 4;
	dma_data[0] = (uint8_t)(m4_address & 0xFF);
	dma_data[1] = (uint8_t)((m4_address >> 8) & 0xFF);
	dma_data[2] = (uint8_t)((m4_address >> 16) & 0xFF);
	dma_data[3] = (uint8_t)((m4_address >> 24) & 0xFF);
	dma_data[4] = (uint8_t)(dmaReadCount & 0xFF);
	dma_data[5] = (uint8_t)((dmaReadCount >> 8) & 0xFF);
	esp32_eoss3_spi_tlc_write(SPITLC_DMA_ADD0, dma_data, 4+2);

	do{
		esp32_eoss3_spi_tlc_read(SPITLC_DMA_STATUS, &dmaStatus, 1);
		if(counter >= MAX_WAIT) return -1;
	} while(((dmaStatus & 0x01) == 1) && (counter++ < MAX_WAIT)); //Wait untill data is ready

	esp32_eoss3_spi_tlc_read(SPITLC_DMA_READ_DATA, buffer, len);
	return len;
}

int esp32_eoss3_spi_ahb_write(uint32_t m4_address, uint8_t* buffer, uint16_t len){

	uint8_t lByte = 0;
	uint8_t* addrPtr;
	int counter = 0;

	do
	{
	esp32_eoss3_spi_tlc_read(SPITLC_AHB_STATUS, &lByte, 1);
	if(counter++ >= MAX_WAIT) return -1;
	} while((lByte & 0x01) != 0);

	lByte = 0x3;
	esp32_eoss3_spi_tlc_write(SPITLC_AHB_ACCESS_CTL, &lByte, 1);

	//make sure last 2 bits are 0x3
	m4_address |= (uint32_t)0x3;
	addrPtr = (uint8_t *)(&m4_address);

	esp32_eoss3_spi_tlc_write(SPITLC_MEM_ADDR_BYTE_0, addrPtr , 4);

	esp32_eoss3_spi_tlc_write(SPITLC_MEM_DATA_BYTE_0, buffer, len);
	return len;
}
