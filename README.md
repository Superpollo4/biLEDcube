# 3x3x3 dual colour LED cube project

Carried on by Alexander J. Magnusson and Jose Cuevas

## Project idea

Our goal is to have a functional 3x3x3 LED cube of Red/Green colours. The cube is supposed to change its colours and animations depending on the input data from a temperature sensor, precisely the DHT12.
To accomplish it there is different stages we had to go through:

- Learn how an LED cube works               [DONE]
- Buy the necessary components              [DONE]
- Build the LED cube                        [DONE]
- Single anode testing                      [DONE]
- Full cube testing with shift registers

## Results

Include gif or video of the outputs we get for the single anode and later full cube testing.

https://i.gifer.com/KHTK.gif

NOT OUR CUBE, ONLY TESTING GIF.

## Components used

- x27 Dual colour R/G LEDs (double anode, three pins)

  BL-BEG204 511-353 [More information](https://www.gme.cz/led-5mm-rg-cc-45-45-50-led-beg204)
 
- x3 SN74HC595 8-bit Shift Register

  [Datasheet](http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=sn74hc595&fileType=pdf)
  
- x18 150-300 Ohms Resistors

  Current limiting. Connected to anodes of LEDs
  
- x1 Arduino UNO microcontroller board

## Acknowledgments

Code from Tomas Fryza has been used and modified for our project. Link to his [gitlab respository](https://gitlab.com/tomas.fryza/avr-examples). 
