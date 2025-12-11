#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "I2C.h"
#include <stdint.h>
#include <stddef.h>

// Adress para el comando 
#define SSD1306_ADR_CMD 	0x00
#define SSD1306_ADR_DATA 	0x40


// ========== CONFIGURACIÓN DEL PANEL ==========
//
#define SSD1306_WIDTH        128
#define SSD1306_HEIGHT        64     // Cambiar a 32 para pantallas 128x32

#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8
// Funcion del OFFSET
// Si l pantalla se ve recorrida, se debe de poner un OFFSET de 2
#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif

//
// ========== COMANDOS BÁSICOS ==========
//
#define SSD1306_DISPLAY_OFF            0xAE    // Apaga el display
#define SSD1306_DISPLAY_ON             0xAF    // Enciende el display

//
// ========== CONFIGURACIÓN DEL CLOCK ==========
//
#define SSD1306_SET_DISPLAY_CLOCK_DIV  0xD5    // Configura el reloj del display
#define SSD1306_OSC_FREQ_DEFAULT       0x80    // Valor recomendado

//
// ========== MULTIPLEX (ALTURA DEL PANEL - 1) ==========
//
#define SSD1306_SET_MULTIPLEX_RATIO    0xA8
#define SSD1306_MULTIPLEX_64           0x3F    // Para 128x64
#define SSD1306_MULTIPLEX_32           0x1F    // Para 128x32

//
// ========== DISPLAY OFFSET ==========
//
#define SSD1306_SET_DISPLAY_OFFSET     0xD3
#define SSD1306_DISPLAY_OFFSET_ZERO    0x00

//
// ========== START LINE ==========
//
#define SSD1306_SET_START_LINE_BASE    0x40
#define SSD1306_SET_START_LINE(n)      (SSD1306_SET_START_LINE_BASE | ((n) & 0x3F))

//
// ========== CHARGE PUMP ==========
//
#define SSD1306_CHARGE_PUMP            0x8D
#define SSD1306_CHARGE_PUMP_ON         0x14
#define SSD1306_CHARGE_PUMP_OFF        0x10

//
// ========== MEMORY ADDRESSING ==========
//
#define SSD1306_MEMORY_MODE            0x20
#define SSD1306_ADDR_MODE_HORIZONTAL   0x00
#define SSD1306_ADDR_MODE_VERTICAL     0x01
#define SSD1306_ADDR_MODE_PAGE         0x02

//
// ========== SEGMENT REMAP (HORIZONTAL FLIP) ==========
//
#define SSD1306_SEG_REMAP_NORMAL       0xA0
#define SSD1306_SEG_REMAP_REVERSE      0xA1

//
// ========== COM SCAN DIRECTION (VERTICAL FLIP) ==========
//
#define SSD1306_COM_SCAN_NORMAL        0xC0
#define SSD1306_COM_SCAN_REVERSE       0xC8

//
// ========== COM PINS CONFIG ==========
//
#define SSD1306_SET_COM_PINS           0xDA
#define SSD1306_COM_PINS_128x64        0x12
#define SSD1306_COM_PINS_128x32        0x02

//
// ========== CONTRASTE ==========
//
#define SSD1306_SET_CONTRAST           0x81
#define SSD1306_DEFAULT_CONTRAST       0x7F

//
// ========== MODOS DE DISPLAY ==========
//
#define SSD1306_DISPLAY_RAM            0xA4    // Usa RAM de la pantalla
#define SSD1306_DISPLAY_ALL_ON         0xA5    // Enciende todos los píxeles
#define SSD1306_DISPLAY_NORMAL         0xA6    // Blanco = ON
#define SSD1306_DISPLAY_INVERTED       0xA7    // Invertido

//
// ========== DIRECCIONES DE COLUMNA ==========
//
#define SSD1306_SET_LOW_COLUMN         0x00    // Nibble bajo
#define SSD1306_SET_HIGH_COLUMN        0x10    // Nibble alto

#define SSD1306_SET_COLUMN_ADDR        0x21    // Define columna inicio/fin
#define SSD1306_SET_PAGE_ADDR          0x22    // Define página inicio/fin

//
// ========== DIRECCIÓN DE PÁGINA (PAGE ADDRESSING MODE) ==========
//
#define SSD1306_SET_PAGE_START_ADDR    0xB0

//
// ========== SCROLL COMPLETO ==========
//
#define SSD1306_SCROLL_RIGHT           0x26
#define SSD1306_SCROLL_LEFT            0x27
#define SSD1306_SCROLL_VERT_RIGHT      0x29
#define SSD1306_SCROLL_VERT_LEFT       0x2A
#define SSD1306_SET_VERT_SCROLL_AREA   0xA3
#define SSD1306_SCROLL_ACTIVATE        0x2F
#define SSD1306_SCROLL_DEACTIVATE      0x2E

//
// ========== MACROS HELPER (AZÚCAR SINTÁCTICA) ==========
//
#define SSD1306_CMD(cmd)               ssd1306_WriteCommand(cmd)
#define SSD1306_CMD2(c1, c2)           { ssd1306_WriteCommand(c1); ssd1306_WriteCommand(c2); }

#define SSD1306_DATA_BYTE(b)           ssd1306_WriteData(&(b), 1)

// Structs 
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SSD1306_t;

/* Font */
typedef struct {
	const uint8_t width;                /**< Font width in pixels */
	const uint8_t height;               /**< Font height in pixels */
	const uint16_t *const data;         /**< Pointer to font data array */
    const uint8_t *const char_width;    /**< Proportional character width in pixels (NULL for monospaced) */
} SSD1306_Font_t;



//
// Funciones 
//
void ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);

#endif // __SSD1306_H__