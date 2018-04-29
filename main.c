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
			if ((UDR0 == '\n') | (rmc_index == 65)){
				gprmc_flag = 0;
				rmc_index = 0;
				
				parse(gps_out,button2);
				//lcd_clrscr();
				//lcd_command(LCD_HOME);
				//lcd_puts(gps_out);
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

void parse(char NMEA[65],int button){
	
	int i,j,k;
	
	char str1[33] = "LAT:            \nLON:            ";//initialize strings
	char str2[33] = "T:       D:      \nSpeed:     Knts";
	
	for(i = 0; i < 65; i++){
		if(NMEA[i] == ',')break;
	}
	
	j = 0;
	for(i++; i < 65; i++){ //time
		if(NMEA[i] == ',') break;
		if (j <= 6) str2[j+2] = NMEA[i];//time
		j++;
	}

	
	for(i++; i < 65; i++){//skip active bit
		if(NMEA[i] == ',')break;
	}
	
	j = 0;
	for(i++; i < 65; i++){//latitude
		if(NMEA[i] == ',')break;
		if (j <= 10) str1[j+5] = NMEA[i];
		j++;
	}
	
	i++;
	j = 0;
	str1[15] = NMEA[i]; //N or S
	
	j = 0;
	for(i += 2; i < 65; i++){ //longitude
		if(NMEA[i] == ',')break;
		if (j <= 10) str1[j+22] = NMEA[i];
		j++;
	}
	
	i++;
	j = 0;
 	str1[32] = NMEA[i];//E or W
	i += 2;
	
	
	j = 0;
	for(i; i < 65; i++){//speed
		if(NMEA[i] == ',')break;
		str2[j+25] = NMEA[i];
		j++;
	}
	
	j = 0;
	for(i++; i < 65; i++){//skip angle
		if(NMEA[i] == ',')break;
	}
	
	j = 0;
	for(i++; i < 65; i++){//date
		if(NMEA[i] == ',')break;
		str2[j+11] = NMEA[i];
		j++;
	}						
				if (button){//display speed, time info if button 2 is pressed
					lcd_command(LCD_HOME);
					for(k = 0; k <= 32; k++){
						lcd_putc(str2[k]);
					}
				}
				else{ 
					lcd_command(LCD_HOME); //else display coordinates
					for(k = 0; k <= 32; k++){
						lcd_putc(str1[k]);
					}
				}
}
	