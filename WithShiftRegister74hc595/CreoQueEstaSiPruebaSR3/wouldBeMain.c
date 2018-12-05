/**
  ******************************************************************************
  * @file    main.c
  * @author  Tomas Fryza, Brno University of Technology, Czechia
  * @version V1.1
  * @date    Oct 04, 2018
  * @brief   Blink LED at PB5 by internal 8-bit Timer/Counter0.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "settings.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

/* Constants and macros ------------------------------------------------------*/
/**
  * @brief Serial data input of 74HC595 shift register.
  */
#define DATA_SHIFT PB0

/**
  * @brief Clock input of 74HC595 shift register.
  */
#define CLK_SHIFT PD7

/**
  * @brief Latch input of 74HC595 shift register.
  */
#define LATCH_SHIFT PD4

/* Function prototypes -------------------------------------------------------*/
void setup(void);
void do_animation(uint8_t, uint8_t, uint8_t);
void segment_toggle_clk(void);


/* Global variables ----------------------------------------------------------*/
/* Animation 1 definition. Index 0 corresponds to SR1, I1 to SR2 and I2 
 * to SR3. Then I3 again to SR1 and so on. This way we send to the SR1 the 0,
 * 3, 6, 9 indexes. Array used to be called segment_digit
 
 * SR1 from R1 to R8
 * Index definition:
 * [0]: All off
 * [1]: All on
 * [2]: X formation (R1, R3, R5, R7 ON)
 * [3]: 
 * rest of indexes in excel book at github repository
 * insert link of github repository
 */
uint8_t SR1_activation[] = {
    0x00, 0xff, 0x55};
	
/* SR2 from G1 to G8
 * Index definition:
 * [0]: All off
 * [1]: All on
 * [2]: X formation (G1, G3, G5, G7 ON)
 * [3]: 
 * rest of indexes in excel book at github repository
 * insert link of github repository
 */
uint8_t SR2_activation[] = {
    0x00, 0xff, 0x55};
/* SR3 from GND1 to GND3 and G9, R9 
 * Index definition:
 * [0]: All off
 * [1]: Layer 1 connected; R9, G9 off
 * [2]: Layer 2 connected; R9, G9 off
 * [3]: Layer 3 connected; R9, G9 off
 * [4]: Layer 1 connected; R9 on
 * rest of indexes in excel book at github repository
 * insert link of github repository
 */
uint8_t SR3_activation[] = {
    0xfc, 0xdc, 0xbc, 0x7c, 0xdd, 0xde, 0xdf, 0xbd, 0xbe, 0xbf, 0x7d, 0x7e, 0x7f};	

 
/* Functions -----------------------------------------------------------------*/
/**
  * @brief Main function.
  */
int main(void)
{
    /* Initializations */
    setup();
	

    /* Show animation number (0 to 0) */
    animation_putc(0);


    /* Enables interrupts by setting the global interrupt mask */
    sei();

    /* Forever loop */
    while (1) {



    }

    return 0;
}

/**
  * @brief Setup all peripherals.
  */
void setup(void)
{
    /* Set output pin 13 (PB5) */
    DDRB |= _BV(PB5);
    DDRB |= _BV(PB4);
    DDRB |= _BV(PB3);
    DDRB |= _BV(PB2);
    DDRB |= _BV(PB1);
    DDRB |= _BV(PB0);
    DDRD |= _BV(PD7);
    DDRD |= _BV(PD6);
    DDRD |= _BV(PD5);
    /* GND layer pins */
    DDRD |= _BV(PD4);
    DDRD |= _BV(PD3);
    DDRD |= _BV(PD2);

    /* Turn LED off */
    PORTB &= ~_BV(PB5);
    PORTB &= ~_BV(PB4);
    PORTB &= ~_BV(PB3);
    PORTB &= ~_BV(PB2);
    PORTB &= ~_BV(PB1);
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD7);
    PORTD &= ~_BV(PD6);
    PORTD &= ~_BV(PD5);
    /* GND layer pins */
    PORTD |=_BV(PD4);
    PORTD |=_BV(PD3);
    PORTD |=_BV(PD2);
	
	/* Set output pins DATA_SHIFT (PB0), CLK_SHIFT (PD7), LATCH_SHIFT (PD4) */
    DDRB |= _BV(DATA_SHIFT);
    DDRD |= _BV(CLK_SHIFT) | _BV(LATCH_SHIFT);
    PORTB &= ~_BV(DATA_SHIFT);
    PORTD &= ~_BV(CLK_SHIFT);
    PORTD &= ~_BV(LATCH_SHIFT);

    /* Timer/Counter0: select clock and enable overflow */
    /* Clock prescaler 1024 => overflows every 16 ms */
    TCCR0B |= _BV(CS02) | _BV(CS00);
    /* Overflow interrupt enable */
    TIMSK0 |= _BV(TOIE0);
}

/**
  * @brief Show digit at position of seven-segment display.
  * @param digit    - Value 0 to 9 to be displayed
  * @param position - Digit position 3 to 0
  */
void do_animation(uint8_t SR1, uint8_t SR2, uint8_t SR3)
{
    uint8_t u8_i;

	/* Animation variable will cycle through animation_1 array */
	/* j will change inside the for loop to jump the specific amount 
	 * for each SR */
	SR1 = SR1_activation[SR1];
	SR2 = SR2_activation[SR2];
	SR3 = SR3_activation[SR3];
	
    /* Shift 16-bit value to shift registers
     * First byte represents digit value, second byte represents position
	 * Rx: active high; Gx: active high; GNDx: active low; x: non relevant (1)
     * SR1 order: R8 R7 R6 R5   R4   R3  R2 R1
     * SR2 order: G7 G6 G5 G4   G3   G2  G1 R9
	 * SR3 order: x  x  x GND3 GND2 GND1 G9 G8
	
	/* Load bits for SR1 */
    for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((SR1 << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
		
    }
	/* Load bits for SR2 */
	for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((SR2 << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
		
    }
	/* Load bits for SR3 */
	for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((SR3 << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
		
    }
	
    /* Set latch input of shift register to high */
	/* With the bytes loaded, latch "sends them" to the corresponding outputs*/
    PORTD |= _BV(LATCH_SHIFT);
    _delay_us(1);

    /* Set latch input of shift register to low */
    PORTD &= ~_BV(LATCH_SHIFT);
}

/**
  * @brief Toggle clock input of shift register.
  */
void segment_toggle_clk(void)
{
    /* Set clock input to high */
    PORTD |= _BV(CLK_SHIFT);
    _delay_us(1);

    /* Set clock input to low */
    PORTD &= ~_BV(CLK_SHIFT);
    _delay_us(1);
}


/**
  * @brief Toggle LED.
  */
ISR(TIMER0_OVF_vect)
{
    do_animation(1,0,4);
	_delay_ms(200);
    do_animation(0,0,0);
	_delay_ms(200);
	do_animation(1,0,7);
	_delay_ms(200);
    do_animation(0,0,0);
	_delay_ms(200);
	


}


/* END OF FILE ****************************************************************/
