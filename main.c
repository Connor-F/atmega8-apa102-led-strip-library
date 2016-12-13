#include "APA102.h"
#include <stdlib.h>
#include <util/delay.h>

#define NUMBER_OF_LEDS 60

int main()
{
	initAPA102(NUMBER_OF_LEDS);

	colour_t *cols[NUMBER_OF_LEDS];
	for(uint16_t i = 0; i < NUMBER_OF_LEDS; i++)
	{
		cols[i] = malloc(sizeof(colour_t));
		if(i == 5)
		{
			cols[i]->red = 0x00;
			cols[i]->green = 0xff;
			cols[i]->blue = 0x00;
			cols[i]->brightness = 3;
		}
		else
		{
			cols[i]->red = i * 3;
			cols[i]->green = i * 4;
			cols[i]->blue = i * 2;
			cols[i]->brightness = 3;
		}
	}

	while(1)
	{
		apa102LightLEDs(cols, NUMBER_OF_LEDS);
		_delay_ms(100);
	}

	/*for(uint16_t i = 0; i < NUMBER_OF_LEDS; i++)
		free(cols[i]);*/

	return 0;
}
