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

static void ssd1306_WriteCommand(uint8_t cmd){
    I2C_WriteCommand(cmd);
}

static void ssd1306_WriteData(uint8_t* buffer, size_t buff_size) {
  I2C_Write_Buffer(buffer, buff_size);
}


void ssd1306_Init(void) {

    delay_ms(100);
	
		//ssd1306_WriteCommand(0xE3);   // SW reset (si tu controlador lo soporta)
		//delay_ms(10);


    ssd1306_WriteCommand(SSD1306_DISPLAY_OFF); // Display OFF

    ssd1306_WriteCommand(SSD1306_SET_DISPLAY_CLOCK_DIV); // Display clock divide
    ssd1306_WriteCommand(0x80);

    ssd1306_WriteCommand(SSD1306_SET_MULTIPLEX_RATIO); // Multiplex ratio
    ssd1306_WriteCommand(SSD1306_HEIGHT - 1);

    ssd1306_WriteCommand(SSD1306_SET_DISPLAY_OFFSET ); // Display offset
    ssd1306_WriteCommand(0x00);

    ssd1306_WriteCommand(0x40); // Start line = 0

    ssd1306_WriteCommand(SSD1306_CHARGE_PUMP); // Charge pump
    ssd1306_WriteCommand(SSD1306_CHARGE_PUMP_ON);

    ssd1306_WriteCommand(SSD1306_MEMORY_MODE); // Memory mode
    ssd1306_WriteCommand(SSD1306_ADDR_MODE_HORIZONTAL); // Horizontal

		
		ssd1306_WriteCommand(SSD1306_SEG_REMAP_REVERSE); // Segment remap
		ssd1306_WriteCommand(0xC8); // COM scan direction
		

    ssd1306_WriteCommand(SSD1306_SET_COM_PINS); // COM pins
    ssd1306_WriteCommand(SSD1306_COM_PINS_128x64);

    ssd1306_WriteCommand(SSD1306_SET_CONTRAST); // Contrast
    ssd1306_WriteCommand(SSD1306_DEFAULT_CONTRAST);

    ssd1306_WriteCommand(0xD9); // Precharge
    ssd1306_WriteCommand(0xF1);
		//ssd1306_WriteCommand(0x22);
		
		
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
    
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_WriteCommand(SSD1306_SET_PAGE_START_ADDR + i); // Set the current RAM page address.
        ssd1306_WriteCommand(0x00 + SSD1306_X_OFFSET_LOWER);
        ssd1306_WriteCommand(0x10 + SSD1306_X_OFFSET_UPPER);
        ssd1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    }
    // Draw color
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
    
    // Proportion char
    const uint8_t char_width = Font.char_width ? Font.char_width[ch-32] : Font.width;
    // Check remaining space on current line
    if (SSD1306_WIDTH < (SSD1306.CurrentX + char_width) ||
        SSD1306_HEIGHT < (SSD1306.CurrentY + Font.height))
    {
        // Not enough space 
        return 0;
    }
    
    // Print font
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
		//Fill with black
		if(color == Black)
			memset(SSD1306_Buffer, 0x00, sizeof(SSD1306_Buffer));
		//Fill with white
		else
			memset(SSD1306_Buffer, 0xFF, sizeof(SSD1306_Buffer));
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

void ssd1306_DrawImage(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_COLOR color) {
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    }

    for (uint8_t j = 0; j < h; j++, y++) {
        for (uint8_t i = 0; i < w; i++) {
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }

            if (byte & 0x80) {
                ssd1306_DrawPixel(x + i, y, color);
            }
        }
    }
    return;
}