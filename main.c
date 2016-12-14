#include "PatternCreator.h"

#include <string.h> // memmove
#include <util/delay.h>

#define NUMBER_OF_LEDS 60

int main()
{
	initPatterGenerator(1, NUMBER_OF_LEDS);

	colour_t cols[NUMBER_OF_LEDS];
	//createRainbow(cols, NUMBER_OF_LEDS);

	while(1)
	{
		//christmasColours(cols);
		//colourExplosion(cols, 0);
		apa102LightLEDs(cols, NUMBER_OF_LEDS);
		_delay_ms(50);


	/*	apa102LightLEDs(cols, NUMBER_OF_LEDS);
		// shift elements by 1 place right (moving effect)
		colour_t temp = cols[NUMBER_OF_LEDS - 1];
		memmove(cols + 1, cols, sizeof(cols) - sizeof(cols[0]));
		cols[0] = temp;*/

		//_delay_ms(5);
	}

	return 0;
}
