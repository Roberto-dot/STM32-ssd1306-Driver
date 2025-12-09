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
	delay_ms(1000);
	
	
	while(1){
	ssd1306_Fill(Black);     // Clear buffer
	ssd1306_SetCursor(0, 0);
	ssd1306_DrawRectangle(128, 64, White);
	ssd1306_SetCursor(5, 5);
		ssd1306_DrawRectangle(90, 55, White);

  ssd1306_UpdateScreen();  // Send buffer to display
	
	}
}