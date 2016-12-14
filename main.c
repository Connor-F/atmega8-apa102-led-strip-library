#include "APA102.h"
#include <string.h> // memmove
#include <util/delay.h>

#define NUMBER_OF_LEDS 60

/*
   fills the provided colour_t array with a continous sequence of
   changing colours
	Param: *dest -> a colour_t array to fill with colours of the rainbow
		   len -> the size of the colour array
*/
void createRainbow(colour_t *dest, uint16_t len)
{
	uint8_t seed = 53 >> 4; // only interresting in top 4 bits
	for(uint16_t i = 0; i < len; i++)
	{
		uint8_t p = seed - i * 8;
		dest[i] = hsvToRgb((uint32_t)p * 359 / 256, 255, 255);
	}
}

int main()
{
	initAPA102(NUMBER_OF_LEDS);

	colour_t cols[NUMBER_OF_LEDS];
	createRainbow(cols, NUMBER_OF_LEDS);

	while(1)
	{
		apa102LightLEDs(cols, NUMBER_OF_LEDS);

		// shift elements by 1 place right (moving effect)
		colour_t temp = cols[NUMBER_OF_LEDS - 1];
		memmove(cols + 1, cols, sizeof(cols) - sizeof(cols[0]));
		cols[0] = temp;

		_delay_ms(5);
	}

	return 0;
}
