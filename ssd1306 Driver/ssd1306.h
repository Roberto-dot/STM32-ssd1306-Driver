#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "I2C.h"
#include <stdint.h>
#include <stddef.h>

// ========== PANEL CONFIGURATION ==========

// Display size
#define SSD1306_WIDTH        128
#define SSD1306_HEIGHT        64    // Change to 32 for 128x32 displays

// Display buffer size (1 bit per pixel)
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8

// X-offset handling (used when the screen appears shifted)
#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif

// ========== BASIC COMMANDS ==========
#define SSD1306_DISPLAY_OFF            0xAE    // Turn display off
#define SSD1306_DISPLAY_ON             0xAF    // Turn display on

// ========== CLOCK CONFIG ==========
#define SSD1306_SET_DISPLAY_CLOCK      0xD5    // Set display clock
#define SSD1306_OSC_FREQ               0x80    // Recommended oscillator value

// ========== MULTIPLEX RATIO ==========
#define SSD1306_SET_MULTIPLEX_RATIO    0xA8    // Set panel height - 1

// ========== DISPLAY OFFSET ==========
#define SSD1306_SET_DISPLAY_OFFSET     0xD3

// ========== START LINE ==========
#define SSD1306_SET_START_LINE_BASE    0x40
#define SSD1306_SET_START_LINE(n)      (SSD1306_SET_START_LINE_BASE | ((n) & 0x3F))

// ========== CHARGE PUMP ==========
#define SSD1306_CHARGE_PUMP            0x8D
#define SSD1306_CHARGE_PUMP_ON         0x14
#define SSD1306_CHARGE_PUMP_OFF        0x10

// ========== MEMORY ADDRESSING MODES ==========
#define SSD1306_MEMORY_MODE            0x20
#define SSD1306_ADDR_MODE_HORIZONTAL   0x00
#define SSD1306_ADDR_MODE_VERTICAL     0x01
#define SSD1306_ADDR_MODE_PAGE         0x02

// ========== SEGMENT REMAP (HORIZONTAL MIRROR) ==========
#define SSD1306_SEG_REMAP_NORMAL       0xA0
#define SSD1306_SEG_REMAP_REVERSE      0xA1

// ========== COM SCAN DIRECTION (VERTICAL FLIP) ==========
#define SSD1306_COM_SCAN_NORMAL        0xC0
#define SSD1306_COM_SCAN_REVERSE       0xC8

// ========== COM PINS CONFIGURATION ==========
#define SSD1306_SET_COM_PINS           0xDA
#define SSD1306_COM_PINS_128x64        0x12
#define SSD1306_COM_PINS_128x32        0x02

// ========== CONTRAST ==========
#define SSD1306_SET_CONTRAST           0x81
#define SSD1306_DEFAULT_CONTRAST       0x7F

// ========== DISPLAY MODES ==========
#define SSD1306_DISPLAY_RAM            0xA4    // Use RAM content
#define SSD1306_DISPLAY_ALL_ON         0xA5    // Force all pixels on
#define SSD1306_DISPLAY_NORMAL         0xA6    // Normal mode (white = on)
#define SSD1306_DISPLAY_INVERTED       0xA7    // Inverted mode

// ========== COLUMN ADDRESSING ==========
#define SSD1306_SET_LOW_COLUMN         0x00
#define SSD1306_SET_HIGH_COLUMN        0x10

#define SSD1306_SET_COLUMN_ADDR        0x21    // Set start/end columns
#define SSD1306_SET_PAGE_ADDR          0x22    // Set start/end pages

// ========== PAGE ADDRESSING ==========
#define SSD1306_SET_PAGE_START_ADDR    0xB0

// ========== FULL SCROLL COMMANDS ==========
#define SSD1306_SCROLL_RIGHT           0x26
#define SSD1306_SCROLL_LEFT            0x27
#define SSD1306_SCROLL_VERT_RIGHT      0x29
#define SSD1306_SCROLL_VERT_LEFT       0x2A
#define SSD1306_SET_VERT_SCROLL_AREA   0xA3
#define SSD1306_SCROLL_ACTIVATE        0x2F
#define SSD1306_SCROLL_DEACTIVATE      0x2E

// ========== COLOR ENUM ==========
typedef enum {
    Black = 0x00,  // Pixel off
    White = 0x01   // Pixel on
} SSD1306_COLOR;

// ========== SCREEN STATE STRUCT ==========
typedef struct {
    uint16_t CurrentX;     // Current X position
    uint16_t CurrentY;     // Current Y position
    uint8_t Initialized;   // Init flag
    uint8_t DisplayOn;     // Display ON/OFF state
} SSD1306_t;

// ========== FONT STRUCTURE ==========
typedef struct {
    const uint8_t width;               // Character width
    const uint8_t height;              // Character height
    const uint16_t *const data;        // Font data array
    const uint8_t *const char_width;   // Variable character widths (NULL = fixed)
} SSD1306_Font_t;

// ========== FUNCTION PROTOTYPES ==========
void ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_DrawRectangle(uint8_t Base, uint8_t High, SSD1306_COLOR color);

#endif // __SSD1306_H__
