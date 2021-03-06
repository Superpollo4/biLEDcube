# 3x3x3 dual colour LED cube project

Carried on by Jose Cuevas Mauricio and Alexander J. Magnusson.

Working code for single anode testing is in singleAnodeTesting folder.

Code still working on for full 3x3x3 LED cube with Shift Registers in WithShiftRegister74hc595 folder.

[Presentation online link (Google Drive)](https://docs.google.com/presentation/d/18E_V4hHT8dRD3xT8ohnPbt7g9iQilgO-eGOgdoE3oGU/edit?usp=sharing)

## Project idea

Our goal is to have a functional 3x3x3 LED cube of Red/Green colours. The cube is supposed to change its colours and animations depending on the input data from a temperature sensor, precisely the DHT12.
To accomplish it there is different stages we had to go through:

- [x] Learn how an LED cube works               
- [x] Buy the necessary components              
- [x] Build the LED cube                        
- [x] Single anode testing                      
- [ ] Full cube testing with shift registers

## Results

Below it is shown the first test with one single anode connected. Corresponds to the code for singleAnodeTest folder.
Better recorded, you can see in the screen the change from 27ºC to 28ºC modifies the animation of the cube:
![](MuestraSingleAnode2.gif)

[Entire video](https://youtu.be/NmyFaFDhJ60)


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
