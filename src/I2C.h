#ifndef I2C_H
#define I2C_H
#include <stm32f446xx.h>

// System clock frequency used for delay
#define system_clock 16000000

// SSD1306 I2C device address
#define SSD1306_I2C_ADDR (0x3C << 1)

// ========== TRANSFER CONTROL (I2C) ==========
#define SSD1306_ADR_CMD   0x00
#define SSD1306_ADR_DATA  0x40

/* I2C write functions */
extern void I2C_Write_Buffer(uint8_t *buffer, uint16_t Buf_size);   
extern void I2C_WriteCommand(uint8_t cmd);                          
extern void delay_ms(uint32_t);                                     
#endif // I2C_H