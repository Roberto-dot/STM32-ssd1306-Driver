#include "conf.h"

void confRCC(void){
    // Enable clocks for GPIOB (bit1) and GPIOC (bit2)
	RCC->AHB1ENR |= (1<<1) | (1<<2);
	
	// Enable clock for I2C1 peripheral
	RCC->APB1ENR |= (1<<21);
	
	// Enable clock for ADC1 peripheral
	RCC->APB2ENR |= (1 << 8);
}

void confGPIO(void) {
    // --- Configure PB6 (SCL) and PB7 (SDA) ---

    // Set pins to Alternate Function mode
    GPIOB->MODER |= (2 << (6*2)) | (2 << (7*2));
    
    // Set Open-Drain output type (required for I2C)
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);
    
    // High speed for I2C lines
    GPIOB->OSPEEDR |= (3 << (6*2)) | (3 << (7*2));
    
    // Enable internal pull-up resistors
    GPIOB->PUPDR |= (1 << (6*2)) | (1 << (7*2));
    
    // Set Alternate Function 4 (AF4) for I2C1 on PB6/PB7
    GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));
}

void confI2C(void) {
    // Reset the I2C peripheral to ensure a clean start
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    
    // Set APB1 peripheral clock frequency to 16 MHz
    I2C1->CR2 |= 16;
    
    // Set I2C speed to 100 kHz (Standard Mode)
    I2C1->CCR |= 80;
    
    // Configure maximum rise time for Standard Mode
    I2C1->TRISE |= 17;
    
    // Enable the I2C1 peripheral
    I2C1->CR1 |= I2C_CR1_PE;
}

// Set general function 
void setConf(void){
	confRCC();
	confGPIO();
	confI2C();
}