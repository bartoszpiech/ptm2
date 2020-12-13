FILENAME   = main
DEVICE     = atmega32
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)
default: compile 

HD44780:
	$(COMPILE) -c HD44780.c -o HD44780.o
compile: HD44780
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o HD44780.o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

clean:
	rm *.o
	rm *.elf
	rm *.hex
