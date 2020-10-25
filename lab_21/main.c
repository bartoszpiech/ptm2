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

char __keypad[4][4] = {
	{ '7', '8', '9', '-' },
	{ '4', '5', '6', '+' },
	{ '1', '2', '3', 'C' },
	{ '*', '0', '#', '=' },
};

char keypad[3][4] = {
	{ '3', '6', '9', '#' },
	{ '2', '5', '8', '0' },
	{ '1', '4', '7', '*' },
};

int _keypad[4][4] = {
	{ 7,	 8,	 9,	 '-' },
	{ 4,	 5,	 6,	 '+' },
	{ 1,	 2,	 3,	 'C' },
	{ '*', 0,	'#', '=' },
};

int diodes[8] = {0};

void set_led(uint8_t led, uint8_t stan);
char get_key();

int main() {
	DDRD = 0xFF;
	//DDRC = 0x00;
	while(1) {
		char num = get_key();
		switch(num) {
			case '0':
				set_led(0, 1);
				break;
			case '1':
				set_led(1, 1);
				break;
			case '2':
				set_led(2, 1);
				break;
			case '3':
				set_led(3, 1);
				break;
			case '4':
				set_led(4, 1);
				break;
			case '5':
				set_led(5, 1);
				break;
			case '6':
				set_led(6, 1);
				break;
			case '7':
				set_led(7, 1);
				break;
		}
		/*
		if (num > -1 && num < 9) {
			diodes[num] = !diodes[num];
			set_led(num, diodes[num]);
		} else {
			switch(num) {
				case '*':
					for (int i = 0; i < 8; i++) {
						diodes[i] = 1;
						set_led(i, diodes[i]);
					}
					break;
				case '#':
					for (int i = 0; i < 8; i++) {
						diodes[i] = 0;
						set_led(i, diodes[i]);
					}
					break;
			}
		}
		*/
	}
}

void set_led(uint8_t led, uint8_t stan) {
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

/*
char get_key() {
	for (int i = 0; i < 4; i++) {
		sbi(PORTC, i + 4);
		for (int row = 0; row < 4; row++) {
			if (bit_is_set(PINC, row)) {
				while(!bit_is_clear(PINC, row)) {
					_delay_ms(10);
				}
				return keypad[i][row];
			}
			cbi(PORTC, i + 4);
		}
	}
	return -1;
}
*/

char get_key() {
	for (int i = 0; i < 3; i++) {
		sbi(PORTC, i + 4);
		for (int row = 0; row < 4; row++) 
			if (bit_is_set(PINC, row)) {
				while(!bit_is_clear(PINC, row))
					_delay_ms(10);
				return keypad[i][row];
			}
		cbi(PORTC, i+4);
	}
	return 'X';
}
