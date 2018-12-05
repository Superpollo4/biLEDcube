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
void animation_putc(uint8_t);
void segment_toggle_clk(void);


/* Global variables ----------------------------------------------------------*/
/* Animation 1 definition. Index 0 corresponds to SR1, I1 to SR2 and I2 
 * to SR3. Then I3 again to SR1 and so on. This way we send to the SR1 the 0,
 * 3, 6, 9 indexes. Array used to be called segment_digit*/
uint8_t animation_1[] = {
    0xff, 0x01, 0xf8, 0xff, 0x01, 0xf4, 0xff, 0x01, 0xec};

/* Digit positions 0 to 3, digit active if log 1*/
uint8_t segment_position[] = {
    0xf1, 0xf2, 0xf4, 0xf8};

volatile uint8_t u8_position_to_show = 0u;
volatile uint8_t u8_counter = 0u;
volatile uint8_t u8_new_sample = 0u;
volatile uint8_t number[2];
 
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
void animation_putc(uint8_t animation)
{
    uint8_t u8_i;
	uint8_t j = animation;

	/* Animation variable will cycle through animation_1 array */
	/* j will change inside the for loop to jump the specific amount 
	 * for each SR */
	animation = animation_1[j];


    /* Shift 16-bit value to shift registers
     * First byte represents digit value, second byte represents position
	 * Rx: active high; Gx: active high; GNDx: active low; x: non relevant (1)
     * SR1 order: R8 R7 R6 R5   R4   R3  R2 R1
     * SR2 order: G7 G6 G5 G4   G3   G2  G1 R9
	 * SR3 order: x  x  x GND3 GND2 GND1 G9 G8
	
	/* SR1: Red anodes from R1 to R8 */
    for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((animation << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
		/* Making j jump 3 indexes to send the value for SR1 */
		j = j + 3;
		if (j == 6)
		{
			u8_i = 8u;
		}
    }
	
	/* Restart j and animation with the first index for the SR2 */
	j = 1;
	animation = animation_1[j];
	
	/* SR2: Red anode R9 and Green anodes from G1 to G7 */
	for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((animation << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
		/* Making j jump 3 indexes to send the value for SR2 */
		j = j + 3;
		if (j == 7)
		{
			u8_i = 8u;
		}
    }
	/* Restart j and animation with the second index for the SR3 */
	j = 2;
	animation = animation_1[j];
	
	/* SR3: Green anodes G8 and G9 PLUS the GND layers. First 3 bits non relevant */
	for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((animation << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
		/* Making j jump 3 indexes to send the value for SR2 */
		j = j + 3;
		if (j == 8)
		{
			u8_i = 8u;
		}
    }
	/* Here we load the value of the digit we want to show into the shift register
	storage. The data is sent to DATA_SHIFT pin (pin 8) and then with the clk
	change it shifts the data value into the storage of the shift register*/
    for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((digit << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT); //write me a 1 on pin 8
        else
            PORTB &= ~_BV(DATA_SHIFT); //or a 0

        _delay_us(1);
        segment_toggle_clk(); //send it to the shift register and shift last value to the right
    }
	/* Here we load the second byte corresponding to the position we want to
	show the digit at. The pos. shift register goes 2nd because we are shifting the
	data input to the right, so we are pushing each bit through SR1 (the pos. SR)
	and later through SR2 (the digits SR) until QH is reached and the for loop ends*/
    for(u8_i = 0u; u8_i < 8u; u8_i++)
    {
        if((position << u8_i) & 0x80u)
            PORTB |= _BV(DATA_SHIFT);
        else
            PORTB &= ~_BV(DATA_SHIFT);

        _delay_us(1);
        segment_toggle_clk();
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
    animation_putc(a);
	a++;
    u8_position_to_show++;

    if(u8_position_to_show > 1u)
        u8_position_to_show = 0u;

    u8_new_sample = 1u;


}


/* END OF FILE ****************************************************************/
