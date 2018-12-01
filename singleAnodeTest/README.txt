Here it can be found the code for a 3x3x3 LED cube of one color implemented with a DHT12 temperature sensor.
Sensor is capable of reading humidity as well but not used in our project.

The main.c inside src folder is the code used. Before that there is another code called clearedMainCode.c where all the comments and unnecessary line had been erased. This cleared code has not been tested.

If only using the code from the files, take into consideration that BUFFER SIZE OF TRANSMITTER has been changed to 64 bytes rather than the original 32 bytes. This is necessary for proper sending of sensor temperature data.
