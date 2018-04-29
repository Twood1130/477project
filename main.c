#define F_CPU 1000000UL
#define DELAY 1000

#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "lcd.h"

int main(void){
	
	
  char gps_out[65] = "wtf"; //initialize the gps string.
  int gprmc_flag = 0, rmc_index = 0;
  int button1 = 1;
  int button2 = 1;
  
  // Watchdog timer disable
  MCUSR=0;
  wdt_disable(); 

  // Set pins D0, and D2 as output.
  DDRB |= (1 << PIN0) | (1 << PIN2);
  DDRB &= ~(1<<PIN7); //pin B7 for pushbutton 2
  PORTB = (1 << PIN2); //turn on pin D2, this turns on the backlight.

  DDRD &= ~(1<<PIN3); // set Pin D3 as input
  
  //USART init
  UCSR0A |= (1 << U2X0); //enable 2x USART operation
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //set USART to 8-bits
  UBRR0 = 12; //set baud rate to 9600, at 1 mhz
  UCSR0B |= (1 << RXEN0); //enable USART RX
  
  //LCD init
  lcd_init(LCD_DISP_ON_CURSOR_BLINK);
  lcd_puts("1234567890ABCDEF\nGHIJKLMOPQRSTUVW");
  
  while (1) {
	if (UCSR0A & (1 << RXC0)){
		PORTB |= (1 << PIN0);
		
		if (gprmc_flag == 0){
		gps_out[0] = UDR0;
			if (gps_out[0] == 'R') {
				gprmc_flag = 1; //detect start of GPRMC string
				rmc_index += 1;
			}
		} 
		
		else if (gprmc_flag == 1) {
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
			
	}
	else PORTB &= (1 << PIN2); //turn indicator LED off.
	
	button1 = ((PINB >> PIN7) & 1);
	button2 = ((PIND >> PIN3) & 1);
	
	if (button1){ //This toggles the backlight with a button press.
		PORTB ^= (1 << PIN2);
		_delay_ms(250);
	}
  }

return 0;
}

void getDATA (char *NMEA, char *time, char *active, char *LAT, char *LONG, char *speed, char *date) {

	char *start, *end;
	
	start = strchr(NMEA, ','); //Find the beginning field 1
	end = strchr((start += 1), ','); //Find the end of field 1
	*end = 0; //truncate the string
	*time = *start;

	start = (end += 1); //start of next field 
	end = strchr(start, ',');
	*end = 0;
	*active = *start;
	
	start = (end += 1); //start at the beginning of Latitude field
	end = strchr(start, ','); //find the next comma 
	end = strchr(++end, ','); //find next comma to include the cardinal direction 
	*end = 0; //zero terminate
	*LAT = *start; //assign leftover string to variable. 
	
	start = (end += 1); //find the start of the Longitude field 
	end = strchr(start, ',');
	end = strchr(++end, ','); //include the cardinal direction. 
	*end = 0;
	*LONG = *start;
	
	start = (end += 1);
	end = strchr(start, ',');
	*end = 0;
	*speed = *start; 
	
	start = strchr((end += 1), ',');
	start += 1;
	end = strchr(start, ',');
	*end = 0;
	*date = *start;
	
	//use atof(start) if need to convert to float. 
}	
	