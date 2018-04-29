#define F_CPU 1000000UL
#define DELAY 1000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "lcd.h"

int main(void){
  char gps_out[65] = "wtf";
  int gprmc_flag = 0, rmc_index = 0;
  // Watchdog timer disable
  MCUSR=0;
  wdt_disable(); 

  // Set pin 0 as output, 1 as input
  DDRB |= (1 << PIN0) | (1 << PIN2);
  PORTB = (1 << PIN2); //turn on backlight

  //USART init
  UCSR0A |= (1 << U2X0);
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //set USART to 8-bits
  UBRR0 = 12; //set baud rate to 9600, at 1.8432 mhz
  
  lcd_init(LCD_DISP_ON_CURSOR_BLINK);
  lcd_puts("1234567890ABCDEF\nGHIJKLMOPQRSTUVW");
  
  UCSR0B |= (1 << RXEN0);
  while (1) {
	if (UCSR0A & (1 << RXC0)){
		PORTB |= (1 << PIN0);
		
		if (gprmc_flag == 0){
			if (UDR0 == 'R') gprmc_flag = 1; //detect start of GPRMC string
		} 
		
		if (gprmc_flag == 1) {
			gps_out[rmc_index] = UDR0;
			rmc_index += 1;
			if ((UDR0 == '\n') | (rmc_index == 16)){
				gprmc_flag = 0;
				rmc_index = 0;
				
				//lcd_clrscr();
				lcd_command(LCD_HOME);
				lcd_puts(gps_out);
			}
		}
			
		
//		if ((gprmc_flag == 1) && (gps_out == ',')){ //comma indexed value finder
//			commas += 1;
//			
//		}
	}
	else PORTB &= 0b00000100;
	

	
	//_delay_ms(500);
  }

return 0;
}