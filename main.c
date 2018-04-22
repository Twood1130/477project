#define F_CPU 1000000UL
#define DELAY 1000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

int main(void){
  
  // Watchdog timer disable
  MCUSR=0;
  wdt_disable(); 

  // Set pin 0 as output, 1 as input
  DDRD |= (1 << PIN7);

  while (1) {
	PORTD ^= (1 << PIN7);
	_delay_ms(500);
  }

return 0;
}