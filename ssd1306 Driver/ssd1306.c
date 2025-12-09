#include "ssd1306.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


// Variables SSD1306_Font_t

static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];
static SSD1306_t SSD1306;

/*Funciones propias o internas*/
static void ssd1306_WriteCommand(uint8_t byte);
static void ssd1306_WriteData(uint8_t* buffer, size_t buff_size);
static void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);

static void ssd1306_WriteCommand(uint8_t cmd){
    I2C_WriteCommand(cmd);
}

static void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
  I2C_Write_Buffer(buffer, buff_size);
}


void ssd1306_Init(void) {

    delay_ms(100);

    ssd1306_WriteCommand(SSD1306_DISPLAY_OFF); 

    ssd1306_WriteCommand(SSD1306_SET_DISPLAY_CLOCK);
    ssd1306_WriteCommand(SSD1306_OSC_FREQ);

    ssd1306_WriteCommand(SSD1306_SET_MULTIPLEX_RATIO); 
    ssd1306_WriteCommand(SSD1306_HEIGHT - 1);

    ssd1306_WriteCommand(SSD1306_SET_DISPLAY_OFFSET); 
    ssd1306_WriteCommand(0x00); // No offset

    ssd1306_WriteCommand(SSD1306_SET_START_LINE_BASE); // Start line = 0

    ssd1306_WriteCommand(SSD1306_CHARGE_PUMP); // Charge pump
    ssd1306_WriteCommand(SSD1306_CHARGE_PUMP_ON);

    ssd1306_WriteCommand(SSD1306_MEMORY_MODE); 
    ssd1306_WriteCommand(SSD1306_ADDR_MODE_HORIZONTAL); 

    ssd1306_WriteCommand(SSD1306_SEG_REMAP_REVERSE); // Segment remap
    ssd1306_WriteCommand(0xC8); // COM scan direction

    ssd1306_WriteCommand(SSD1306_SET_COM_PINS); 
    ssd1306_WriteCommand(SSD1306_COM_PINS_128x64);

    ssd1306_WriteCommand(SSD1306_SET_CONTRAST); 
    ssd1306_WriteCommand(SSD1306_DEFAULT_CONTRAST);

    ssd1306_WriteCommand(0xD9); // Precharge
    ssd1306_WriteCommand(0xF1);
    // VCOMH controla el voltaje máximo que el driver 
    ssd1306_WriteCommand(0xDB); // VCOMH deselect
    ssd1306_WriteCommand(0x40);

    ssd1306_WriteCommand(0xA4); // Entire display ON (resume RAM)
    ssd1306_WriteCommand(0xA6); // Normal display

    ssd1306_WriteCommand(SSD1306_DISPLAY_ON); // Display ON

    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
    
    SSD1306.Initialized = 1;
}

void ssd1306_UpdateScreen(void) {
    // Write data to each page of RAM. Number of pages
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_WriteCommand(SSD1306_SET_PAGE_START_ADDR + i); // Set the current RAM page address.
        ssd1306_WriteCommand(0x00 + SSD1306_X_OFFSET_LOWER);
        ssd1306_WriteCommand(0x10 + SSD1306_X_OFFSET_UPPER);
        ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

static void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        // Don't write outside the buffer
        return;
    }
    // Draw in the right color
    if(color == White) 
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    if(color == Black) 
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color) {
    uint32_t i, b, j;
    
    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;
    
    // Char width is not equal to font width for proportional font
    const uint8_t char_width = Font.char_width ? Font.char_width[ch-32] : Font.width;
    // Check remaining space on current line
    if (SSD1306_WIDTH < (SSD1306.CurrentX + char_width) ||
        SSD1306_HEIGHT < (SSD1306.CurrentY + Font.height))
    {
        // Not enough space on current line
        return 0;
    }
    
    // Use the font to write
    for(i = 0; i < Font.height; i++) {
        b = Font.data[(ch - 32) * Font.height + i];
        for(j = 0; j < char_width; j++) {
            if((b << j) & 0x8000)  {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)color);
            } else {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }
    
    // The current space is now taken
    SSD1306.CurrentX += char_width;
    
    // Return written char for validation
    return ch;
}

void ssd1306_SetCursor(uint8_t x, uint8_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

void ssd1306_Fill(SSD1306_COLOR color) {
    memset(SSD1306_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}


char ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
    while (*str) {
        if (ssd1306_WriteChar(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        str++;
    }
    
    // Everything ok
    return *str;
}

void ssd1306_DrawRectangle(uint8_t Base, uint8_t High, SSD1306_COLOR color) {
    uint32_t i;
		if (SSD1306_WIDTH < (SSD1306.CurrentX + Base) ||
        SSD1306_HEIGHT < (SSD1306.CurrentY + High))
    {
        // Not enough space on 2
        return;
    }
    for(i = 0; i < Base; i++){
        ssd1306_DrawPixel(SSD1306.CurrentX + i, SSD1306.CurrentY, color);
        ssd1306_DrawPixel(SSD1306.CurrentX + i, SSD1306.CurrentY + High - 1, color);
    }

    for(i = 0; i < High; i++){
        ssd1306_DrawPixel(SSD1306.CurrentX, SSD1306.CurrentY + i, color);
        ssd1306_DrawPixel(SSD1306.CurrentX + Base - 1, SSD1306.CurrentY + i, color);
    }
}

