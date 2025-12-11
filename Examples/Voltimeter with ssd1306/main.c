#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "conf.h"
#include <stdio.h>

int main(void){
	setConf();
	
	ssd1306_Init();          // Initialize display
	
  ssd1306_Fill(Black);     // Clear buffer
	
  ssd1306_UpdateScreen();  // Send buffer to display
	
	ssd1306_SetCursor(0, 0);
  ssd1306_WriteString("Voltimeter!", Font_11x18, White);
		
	ssd1306_UpdateScreen();  // Send buffer to display

	uint16_t lecADC;;
	char buffer[20];

	ADC1->CR2 |= ADC_CR2_SWSTART;
	
	while(1){
	lecADC = convADC();
	uint16_t entero = lecADC / 10;
	uint16_t decimal = lecADC % 10;
	sprintf(buffer, "%u.%u V", entero, decimal);
		
	//delay_ms(1000);
	
  ssd1306_SetCursor(38, 28);
  ssd1306_WriteString(buffer, Font_11x18, White);
		
	/*ssd1306_SetCursor(43, 28);
  ssd1306_WriteString("V", Font_11x18, White);
	*/
  ssd1306_UpdateScreen();  // Send buffer to display
	delay_ms(100);
	
	}
}