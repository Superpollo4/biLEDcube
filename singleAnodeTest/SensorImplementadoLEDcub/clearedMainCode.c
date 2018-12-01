/**
  ******************************************************************************
  * @file    main.c
  * @author  Alexander J. Magnusson & José Cuevas, based off examples from Tomas Fryza
  * 		 Originally from Tomas Fryza, Brno University of Technology, Czechia
  * @version V1.1
  * @date    Nov 29, 2018
  * @brief   Light up a 3x3x3 single anode LED cube in two different animations 
			 depending on the data received from the DHT12 temperature sensor.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "settings.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "twi.h"
#include "uart.h"


/* Constants and macros ------------------------------------------------------*/
/**
 *  @brief Define UART buad rate.
 */
#define UART_BAUD_RATE 9600
#define DHT12 0x5c


struct values{
    uint8_t temperature_integer;
    uint8_t temperature_decimal;
};
struct values Meteo_values;
/* Function prototypes -------------------------------------------------------*/
/**
 *  @brief Initialize UART, TWI, and Timer/Counter1.
 */
void setup(void);

/**
 *  @brief TWI Finite State Machine transmits all slave addresses.
 */
void fsm_twi_scanner(void);

/* Global variables ----------------------------------------------------------*/
typedef enum {
    IDLE_STATE = 1,
    SLA_W_STATE,
    ACK_STATE,
    TEMPERATURE_STATE,
    UART_STATE,
} state_t;
/* FSM for scanning TWI bus */
state_t twi_state = IDLE_STATE;

/* Functions -----------------------------------------------------------------*/
/**
  * @brief Main function.
  */
int main(void)
{
    /* Initializations */
    setup();

    /* Enables interrupts by setting the global interrupt mask */
    sei();

    /* Forever loop */
    while (1) {
        fsm_twi_scanner();
        if (Meteo_values.temperature_integer <= 28)
        {
            TestAnimation1();
        }
        else 
		{
            TestAnimation2();
        }

    }

    return 0;
}

/**
  * @brief Setup all peripherals.
  */
void setup(void)
{
    /* Initialize UART: asynchronous, 8-bit data, no parity, 1-bit stop */
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    /* Initialize TWI */
    twi_init();

    /* Set LEDs as outputs */
    DDRB |= _BV(PB5);
    DDRB |= _BV(PB4);
    DDRB |= _BV(PB3);
    DDRB |= _BV(PB2);
    DDRB |= _BV(PB1);
    DDRB |= _BV(PB0);
    DDRD |= _BV(PD7);
    DDRD |= _BV(PD6);
    DDRD |= _BV(PD5);
    /* and GND layer pins */
    DDRD |= _BV(PD4);
    DDRD |= _BV(PD3);
    DDRD |= _BV(PD2);

    /* Turn all LEDs off */
    PORTB &= ~_BV(PB5);
    PORTB &= ~_BV(PB4);
    PORTB &= ~_BV(PB3);
    PORTB &= ~_BV(PB2);
    PORTB &= ~_BV(PB1);
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD7);
    PORTD &= ~_BV(PD6);
    PORTD &= ~_BV(PD5);
    /* and GND layer pins. When cleared, they close the circuit
	 * connecting the layer to GND and making it possible for
	 * the LEDs of the specific layer to turn on. */
    PORTD |=_BV(PD4);
    PORTD |=_BV(PD3);
    PORTD |=_BV(PD2);

}


/* LED cube animation of turned on LEDs layer shifting up */
/**
  * function animation test 1
  */
void TestAnimation1()
{
    PORTD &= ~_BV(PD4);
    PORTD |=_BV(PD3);
    PORTD |=_BV(PD2);
    PORTB |=_BV(PB5);
    PORTB |=_BV(PB4);
    PORTB |=_BV(PB3);
    PORTB |=_BV(PB2);
    PORTB |=_BV(PB1);
    PORTB |=_BV(PB0);
    PORTD |=_BV(PD7);
    PORTD |=_BV(PD6);
    PORTD |=_BV(PD5);
    _delay_ms (200);
    PORTB &= ~_BV(PB5);
    PORTB &= ~_BV(PB4);
    PORTB &= ~_BV(PB3);
    PORTB &= ~_BV(PB2);
    PORTB &= ~_BV(PB1);
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD7);
    PORTD &= ~_BV(PD6);
    PORTD &= ~_BV(PD5);
    /* Change of layer */
    PORTD &= ~_BV(PD3);
    PORTD |=_BV(PD4);
    PORTD |=_BV(PD2);
    PORTB |=_BV(PB5);
    PORTB |=_BV(PB4);
    PORTB |=_BV(PB3);
    PORTB |=_BV(PB2);
    PORTB |=_BV(PB1);
    PORTB |=_BV(PB0);
    PORTD |=_BV(PD7);
    PORTD |=_BV(PD6);
    PORTD |=_BV(PD5);
    _delay_ms (200);
    PORTB &= ~_BV(PB5);
    PORTB &= ~_BV(PB4);
    PORTB &= ~_BV(PB3);
    PORTB &= ~_BV(PB2);
    PORTB &= ~_BV(PB1);
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD7);
    PORTD &= ~_BV(PD6);
    PORTD &= ~_BV(PD5);
    /* Change of layer */
    PORTD &= ~_BV(PD2);
    PORTD |=_BV(PD4);
    PORTD |=_BV(PD3);
    PORTB |=_BV(PB5);
    PORTB |=_BV(PB4);
    PORTB |=_BV(PB3);
    PORTB |=_BV(PB2);
    PORTB |=_BV(PB1);
    PORTB |=_BV(PB0);
    PORTD |=_BV(PD7);
    PORTD |=_BV(PD6);
    PORTD |=_BV(PD5);
    _delay_ms (200);
    PORTB &= ~_BV(PB5);
    PORTB &= ~_BV(PB4);
    PORTB &= ~_BV(PB3);
    PORTB &= ~_BV(PB2);
    PORTB &= ~_BV(PB1);
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD7);
    PORTD &= ~_BV(PD6);
    PORTD &= ~_BV(PD5);
    
}
/* LED cube animation of turned on LEDs slices shifting to the side */
/**
  * function animation test 2
  */
void TestAnimation2()
{
    PORTD &= ~_BV(PD4);
    PORTD &= ~_BV(PD3);
    PORTD &= ~_BV(PD2);
    PORTB |=_BV(PB3);
    PORTB |=_BV(PB0);
    PORTD |=_BV(PD5);
    _delay_ms (400);
    PORTB &= ~_BV(PB3);
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD5);
    PORTB |=_BV(PB4);
    PORTB |=_BV(PB1);
    PORTD |=_BV(PD6);
    _delay_ms (400);
    PORTB &= ~_BV(PB4);
    PORTB &= ~_BV(PB1);
    PORTD &= ~_BV(PD6);
    PORTB |=_BV(PB5);
    PORTB |=_BV(PB2);
    PORTD |=_BV(PD7);
    _delay_ms (400);
    PORTB &= ~_BV(PB5);
    PORTB &= ~_BV(PB2);
    PORTD &= ~_BV(PD7);
    
}


void fsm_twi_scanner(void)
{
    /* Static variable inside a function keeps its value between callings */
    //static uint8_t slave_address = 0;
    uint8_t twi_status;
    char uart_string3[10];
    char uart_string4[10];

    switch (twi_state) {
    case IDLE_STATE:
        twi_state = TEMPERATURE_STATE;
        break;
		
    case TEMPERATURE_STATE:        
        twi_status = twi_start((DHT12<<1) + TWI_WRITE);
        if (twi_status==0){
            twi_write (0x02);
            twi_stop ();
            twi_start((DHT12<<1) + TWI_READ);
            Meteo_values.temperature_integer = twi_read_ack();
            Meteo_values.temperature_decimal = twi_read_nack();
            twi_stop ();
            twi_state = UART_STATE;
        }
        else {
            uart_puts("Not connected T");

            twi_state = IDLE_STATE;
        }
        break;

    case UART_STATE:
        uart_puts("\r\n---Temperature values---:\r\n");
        itoa (Meteo_values.temperature_integer, uart_string3, 10);
        itoa (Meteo_values.temperature_decimal, uart_string4, 10);
        uart_puts(uart_string3);
        uart_puts(".");
        uart_puts(uart_string4);
        twi_state = IDLE_STATE;
        break;

    default:
        twi_state = IDLE_STATE;
    } /* End of switch (twi_state) */
}

/* END OF FILE ****************************************************************/
