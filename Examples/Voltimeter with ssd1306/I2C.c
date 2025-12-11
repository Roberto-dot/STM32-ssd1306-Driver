#include "I2C.h"

void I2C_WriteCommand(uint8_t cmd) {

    // START
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    // Address
    I2C1->DR = SSD1306_I2C_ADDR;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Control byte = 0x00 (command)
    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = SSD1306_ADR_CMD;

    // Command
    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = cmd;

    while(!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_Write_Buffer(uint8_t *buffer, uint16_t Buf_size) {

    // START
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = SSD1306_I2C_ADDR;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;

    // Control byte = 0x40 (DATA)
    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = SSD1306_ADR_DATA;

    // Send entire buffer
    for(uint16_t i = 0; i < Buf_size; i++) {
        while(!(I2C1->SR1 & I2C_SR1_TXE));
        I2C1->DR = buffer[i];
    }

    while(!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

void delay_ms(uint32_t ms){
	uint32_t counter;
	while (ms--) {
		counter = system_clock / 1000;
		while(counter--){
        __asm("NOP");
    }
	}
} 
