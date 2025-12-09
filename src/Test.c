#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "conf.h"

int main(void){
	setConf();
	
	ssd1306_Init();          // Initialize display
	
  	ssd1306_Fill(Black);     // Clear buffer
	
  	ssd1306_SetCursor(0, 0);
 	ssd1306_WriteString("Hello!", Font_11x18, White);
	
  	ssd1306_SetCursor(0, 30);
 	ssd1306_WriteString("STM32", Font_11x18, White);
	
  	ssd1306_UpdateScreen();  // Send buffer to display
	delay_ms(500);
	
	while(1){
	ssd1306_Fill(Black);     // Clear buffer
	
  	ssd1306_SetCursor(0, 0);
 	ssd1306_WriteString("Test Font 6x8", Font_6x8, White);
	
  	ssd1306_SetCursor(0, 30);
 	ssd1306_WriteString("Test Font 7x10", Font_7x10, White);
	
	ssd1306_SetCursor(0, 30);
 	ssd1306_WriteString("Test Font Font 11x18", Font_11x18, White);

  	ssd1306_UpdateScreen();  // Send buffer to display
	delay_ms(700);
	
	ssd1306_Fill(Black);     // Clear buffer
	
  	ssd1306_SetCursor(0, 0);
 	ssd1306_WriteString("Test Font 16x24", Font_16x26, White);
	
  	ssd1306_SetCursor(0, 30);
 	ssd1306_WriteString("Test Font 15x24", Font_16x24, White);

  	ssd1306_UpdateScreen();  // Send buffer to display
	
	delay_ms(700);

		ssd1306_Fill(Black);     // Clear buffer
	
  	ssd1306_SetCursor(0, 0);
 	ssd1306_WriteString("Test Rectangles", Font_11x18, White);
	ssd1306_UpdateScreen();  // Send buffer to display

	delay_ms(200);

	// Small rectangle
	ssd1306_SetCursor(0, 20);
	ssd1306_DrawRectangle(30, 10, White);


	// Medium rectangle
	ssd1306_SetCursor(40, 20);
	ssd1306_DrawRectangle(50, 20, White);


	// Large rectangle
	ssd1306_SetCursor(0, 45);
	ssd1306_DrawRectangle(80, 18, White);


	ssd1306_UpdateScreen();
	
	delay_ms(500);
	}
}