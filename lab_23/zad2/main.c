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

void
ADC_init();

uint16_t
ADC_10bit(uint8_t pin_number);

void
two_step(uint8_t requested, uint8_t real);

void
two_step_hyst(uint8_t requested, uint8_t real);

void
three_step(uint8_t requested_1, uint8_t requested_2, uint8_t real);

void set_led(uint8_t led, uint8_t state);


int main() {
    DDRD = 0xFF;
    char text[2][20];
    float voltage[2];
    float tmp;	// potrzebne do 3stawnego
    ADC_init();
    LCD_Initialize();
    LCD_Home();
    while(1) {
	for (int i = 0; i < 2; i++) {
	    voltage[0] = ADC_10bit(i) * 5 / 1024.;
	    voltage[1] = voltage[0] / 5 * 100;
	    dtostrf( voltage[0], 4, 2, text[0]);
	    dtostrf( voltage[1], 4, 2, text[1]);
	    //LCD_Clear();
	    LCD_GoTo(0,i);
	    LCD_WriteText(text[0]);
	    LCD_WriteText("V, ");
	    LCD_WriteText(text[1]);
	    LCD_WriteText("%");
	}
	voltage[0] = ADC_10bit(0) / 1024. * 100;
	voltage[1] = ADC_10bit(1) / 1024. * 100;
	two_step((uint8_t)voltage[0], (uint8_t)voltage[1]);
	two_step_hyst((uint8_t)voltage[0], (uint8_t)voltage[1]);
	tmp = ADC_10bit(2) / 1024. * 100; // to-do 10bit(2);
    }
}

void
ADC_init() { 
    sbi(ADMUX, REFS0);	// ustawienie napiecia avcc
    cbi(ADMUX, REFS1);

    sbi(ADCSRA, ADPS0);	// ustawienie czestotliwosci mniejszej
    sbi(ADCSRA, ADPS1);	// niz 100kHz (8Mhz / 128) = 62,5kHz < 100kHz
    sbi(ADCSRA, ADPS2);

    sbi(ADCSRA, ADEN);	// uruchomienie ukladu przetwornika ADC
}

uint16_t
ADC_10bit(uint8_t pin_number) {
    if (pin_number == 0) {
	cbi(ADMUX, MUX0);
    } else if (pin_number == 1) {
	sbi(ADMUX, MUX0);
    } else {
	return -1;  // blad
    }
    sbi(ADCSRA, ADSC);
    while (bit_is_set(ADCSRA, ADSC)) { }
    return ADC;
}

void
two_step(uint8_t requested, uint8_t real) {
    if (requested >= real) {
	set_led(0, 1);
    } else {
	set_led(0, 0);
    }
}

void
two_step_hyst(uint8_t requested, uint8_t real) {
    uint8_t hysteresis = (0.5 * 100 / 5);	// [V]
    if (abs(requested - real) < hysteresis) {
	return;
    } else if (requested >= real) {
	set_led(1, 1);
    } else {
	set_led(1, 0);
    }
}

void
three_step(uint8_t requested_1, uint8_t requested_2, uint8_t real) {
    if (requested_1 >= real) {
	set_led(2, 1);
    } else if (requested_1 < real) {
	set_led(2, 0);
    }
    if (requested_2 >= real) {
	set_led(3, 0);
    } else if (requested_2 < real) {
	set_led(3, 1);
    }
}


void set_led(uint8_t led, uint8_t state) {
    if (state == 1) {
	sbi(PORTD, led);
    } else {
	cbi(PORTD, led);
    }
}
