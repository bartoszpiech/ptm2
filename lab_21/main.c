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
char pins_c[] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};

char keypad[4][4] = {
	{ '-', '+', 'C', '=' },
	{ '9', '6', '3', '#' },
	{ '8', '5', '2', '0' },
	{ '7', '4', '1', '*' },
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
				case '*':
					for (int i = 0; i < 8; i++) {
						diodes[i] = 1;
						set_led(i+1, diodes[i]);
					}
					break;
				case '#':
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

void _set_led(uint8_t led, uint8_t stan) {
	switch(led) {
		case 0:
			if (stan == 1) {
				sbi(PORTD, PD0);
			} else {
				cbi(PORTD, PD0);
			}
			break;
		case 1:
			if (stan == 1) {
				sbi(PORTD, PD1);
			} else {
				cbi(PORTD, PD1);
			}
			break;
		case 2:
			if (stan == 1) {
				sbi(PORTD, PD2);
			} else {
				cbi(PORTD, PD2);
			}
			break;
		case 3:
			if (stan == 1) {
				sbi(PORTD, PD3);
			} else {
				cbi(PORTD, PD3);
			}
			break;
		case 4:
			if (stan == 1) {
				sbi(PORTD, PD4);
			} else {
				cbi(PORTD, PD4);
			}
			break;
		case 5:
			if (stan == 1) {
				sbi(PORTD, PD5);
			} else {
				cbi(PORTD, PD5);
			}
			break;
		case 6:
			if (stan == 1) {
				sbi(PORTD, PD6);
			} else {
				cbi(PORTD, PD6);
			}
			break;
		case 7:
			if (stan == 1) {
				sbi(PORTD, PD7);
			} else {
				cbi(PORTD, PD7);
			}
			break;
	}
}


char get_key() {
	for (int i = 0; i < 4; i++) {
		sbi(PORTC, i + 4);
		for (int j = 0; j < 4; j++) {
			if (bit_is_set(PINC, pins_c[0])) { // tutaj nie wiem musze sprawdzac PC0
				return keypad[i][0];						 // moze bug simulide
			}
			if (bit_is_set(PINC, pins_c[j])) {
				return keypad[i][j];
			}
		}
		cbi(PORTC, i + 4);
	}
	return 'X';
}
