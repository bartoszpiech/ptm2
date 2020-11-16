#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#ifndef _BV
#define _BV(bit)			(1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)	reg |= (_BV(bit))
#endif
#ifndef cbi
#define cbi(reg,bit)	reg &= ~(_BV(bit))
#endif
#ifndef abi
#define abi(reg,bit)	reg ^= (_BV(bit))
#endif

int diodes[8] = {0};

char keypad[4][4] = {
	{ '*', '0', '#', '=' },
	{ '1', '2', '3', 'C' },
	{ '4', '5', '6', '+' },
	{ '7', '8', '9', '-' },
};

void set_led(uint8_t led, uint8_t stan);
char get_key();

int main() {
	DDRD = 0xFF;
	DDRC = 0x00;
	while(1) {
		char ch = get_key();
		int num = ch - '0';
		if (num > -1 && num < 9) {
			diodes[num] = !diodes[num];
			set_led(num, diodes[num]);
		} else {
			switch(ch) {
				case '#':
					for (int i = 0; i < 8; i++) {
						diodes[i] = 1;
						set_led(i+1, diodes[i]);
					}
					break;
				case '*':
					for (int i = 0; i < 8; i++) {
						diodes[i] = 0;
						set_led(i+1, diodes[i]);
					}
					break;
			}
		}
	}
}

void set_led(uint8_t led, uint8_t stan) {
	if (stan == 1) {
	sbi(PORTD, led-1);
	} else {
		cbi(PORTD, led-1);
	}
}

char get_key() {
	for (int i = 0; i < 4; i++) {
		sbi(PORTC, i + 4);
		for (int j = 0; j < 4; j++) {
			if (bit_is_set(PINC, j)) {
				while(bit_is_set(PINC, j)) {
					_delay_ms(10);
				}
				return keypad[i][j];
			}
		}
		cbi(PORTC, i + 4);
	}
	return 'X';
}
