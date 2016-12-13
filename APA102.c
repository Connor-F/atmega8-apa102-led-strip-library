#include "APA102.h"
#include "SPI.h"
#include <stdlib.h> // NULL

/*
   prepares the apa102 led strip for use (sets up spi)
	Param: leds -> the number of leds in the strip that is being controlled
*/
void initAPA102(uint16_t leds)
{
	numberOfLEDs = leds;
	initSPI();
}

uint8_t apa102LightLEDs(colour_t *col[], uint16_t len)
{
	if(col == NULL)
		return 1;
	if(len > numberOfLEDs)
		return 2;

	// start frame indicator
	spiExchangeByte(0x00);
	spiExchangeByte(0x00);
	spiExchangeByte(0x00);
	spiExchangeByte(0x00);

	for(uint16_t i = 0; i < len; i++)
	{
		if(col[i] == NULL)
			continue;

		// led frame
		spiExchangeByte(0xe0 + col[i]->brightness);
		spiExchangeByte(col[i]->blue);
		spiExchangeByte(col[i]->green);
		spiExchangeByte(col[i]->red);
	}

	// end frame indicator
	spiExchangeByte(0xff);
	spiExchangeByte(0xff);
	spiExchangeByte(0xff);
	spiExchangeByte(0xff);

	return 0;
}
