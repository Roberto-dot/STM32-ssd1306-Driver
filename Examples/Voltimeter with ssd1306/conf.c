#include "conf.h"

void confRCC(void){
	RCC->AHB1ENR |= (1<<1) | (1<<2);
	
	// Encendemos CLK del I2C
	RCC->APB1ENR |= (1<<21);
	
	// Encendemos CLK del ADC1
	RCC->APB2ENR |= (1 << 8);
}

void confGPIO(void) {
    // Configuración para PB6 (SCL) y PB7 (SDA)
    
    // Limpiar bits de configuración
    GPIOB->MODER &= ~((3 << (6*2)) | (3 << (7*2)));
    // Configurar como Alternate Function (10)
    GPIOB->MODER |= (2 << (6*2)) | (2 << (7*2));
    
    // Configurar como Open-Drain (CRÍTICO para I2C)
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);
    
    // Configurar velocidad alta
    GPIOB->OSPEEDR |= (3 << (6*2)) | (3 << (7*2));
    
    // Sin esto, el I2C NO funcionará de forma confiable
    GPIOB->PUPDR |= (1 << (6*2)) | (1 << (7*2));  // Pull-up
    
    // Configurar Alternate Function 4 (I2C1) en PB6 y PB7
    GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));         // AF4
		
		GPIOC->MODER |= (3 << (0 * 2));
}

void confI2C(void) {
    
    // Frecuencia de 1a targeta
    I2C1->CR2 |= 16;              // 16 MHz
    
    // Configurar velocidad del I2C a 100 kHz (Standard Mode)
    I2C1->CCR |= 80;
    
    // Configurar TRISE (Maximum rise time)
    I2C1->TRISE |= 17;
    
    // Habilitar el periférico I2C
    I2C1->CR1 |= I2C_CR1_PE;
}

void confADC(void) {
		// Prescaler para mejora en la recolección de datos
		ADC->CCR |= (1 << 16);
	
		// Sampleo (28 ciclos para canal 10)
		ADC1->SMPR1 |= (3 << 0);
	
    // Seleccionamos canal 10 para la primera conversión
    ADC1->SQR3 = 10;

    // Modo scan (aunque no es necesario si solo hay un canal)
    ADC1->CR1 |= (1 << 8);

    // Encendemos ADC + conversión continua
    ADC1->CR2 |= (3 << 0);
		//ADC1->CR2 |= ADC_CR2_ADON;
	
		//ADC1->CR2 |= ADC_CR2_SWSTART;	

}

uint16_t convADC(void) {
		while (!(ADC1->SR & ADC_SR_EOC));
		uint16_t data;
    uint16_t temp = ADC1->DR;
    //ADC1->SR &= ~ADC_SR_EOC;
	
	data = (temp*333)/4095;

	return data;  
}



void setConf(void){
	confRCC();
	confGPIO();
	confI2C();
	confADC();
}