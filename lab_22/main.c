#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "HD44780.h"

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

enum operation { sum, sub, mul }; // operacje dodawania odejmowania i mnozenia

void set_led(uint8_t led, uint8_t stan);
char get_key();

int main() {
    int flag;
    int num1 = 0, num2 = 0;
    enum operation op = sum;
    int result = 0;
    LCD_Initialize();
    LCD_Home();
    char text[20];
    DDRD = 0xFF;
    DDRC = 0x00;
    while(1) {
	char ch = get_key();
	if (ch != 'X') { // sprawdzanie czy rzeczywiscie kliknieto przycisk
	    flag = 0; // flaga pozwala wyswietlac wyniki po kliknieciu operacji dodawania
	    int num = ch - '0';
	    if (num > -1 && num < 10) {
		LCD_Clear();
		int tmp = num1 % 10; // zmienna pomocnicza
		if (num1 == 0) {
		    num1 = num; // cyfra jednosci
		} else if (num1 < 10 && num1 > 0) {
		    num1 = tmp * 10 + num; // cyfra dziesiatek
		} else if (num1 < 100 && num1 > 9) {
		    num1 = (num1 - tmp) * 10 + num1 % 10 * 10 + num; // cyfra setek
		} else { // wartosci sa zerowane przy probie przekroczenia zakresu ( > 999 )
		    num1 = 0;
		    num2 = 0;
		    result = 0;
		}
	    } else {
		switch (ch) {
		    case 'C': // zerowanie wartosci
			num1 = 0;
			num2 = 0;
			result = 0;
			LCD_Clear();
			break;
		    case '+':
			LCD_Clear();
			op = sum; // ustawienie operatora sumy
			if (num1 != 0) {
			    result = num1 + num2;
			    num2 = result; // "sztuczna pamiec"
			    num1 = 0;
			}
			flag = 1;
			break;
		    case '-':
			LCD_Clear();
			op = sub; // operator odejmowania
			if (num1 != 0) {
			    if (num2 == 0) {
				result = num1;
			    } else {
				result = num2 - num1;
			    }
			    num2 = result;
			    num1 = 0;
			}
			flag = 1;
			break;
		    case '*':
			LCD_Clear();
			op = mul; // operator mnozenia
			if (num1 != 0) {
			    if (num2 == 0) {
				result = num1;
			    } else {
				result = num1 * num2;
			    }
			    num2 = result;
			    num1 = 0;
			}
			flag = 1;
			break;
		    case '=':
			if (op == sum) {
			    result = num1 + num2;
			} else if (op == sub) {
			    result = num2 - num1;
			} else {
			    result = num1 * num2;
			}
			sprintf(text, "Wynik: %d", result);
			LCD_GoTo(0, 1);
			LCD_WriteText(text);
			//num2 = 0;
			break;
		}
	    }
	    //sprintf(text, "1:%d 2:%d", num1, num2); debug
	    if (flag == 0) { // jesli nacisnieto "="
		sprintf(text, "%d", num1);
	    } else { // jesli nacisnieto "+" lub "-"
		sprintf(text, "%d", result);
	    }
	    LCD_GoTo(0, 0);
	    LCD_WriteText(text);
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

