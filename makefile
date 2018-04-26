TARGET = main

CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atmega88

OBJCOPY=avr-objcopy
ADFLAGS=-p m88 -c usbtiny -P usb

.PHONY : flash erase clean

$(TARGET).obj : $(TARGET).o
	$(CC) $(CFLAGS) lcd.c $< -o $@

$(TARGET).hex: $(TARGET).obj
	$(OBJCOPY) -R .eeprom -O ihex $< $@

$(TARGET).eeprom : $(TARGET).obj
	$(OBJCOPY) -j .eeprop -O ihex $< $@

flash: $(TARGET).hex $(TARGET).eeprom
	avrdude $(ADFLAGS) -U flash:w:$(TARGET).hex:i -F -v -v -v -v
	
erase:
	avrdude $(ADFLAGS) -F -e -v -v

clean:
	rm -f *~ *.hex *.obj *.o *.eeprom

fuses:
	avrdude $(ADFLAGS) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m

