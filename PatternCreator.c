#include "PatternCreator.h"
#include <stdlib.h> // rand()

static uint8_t loopCount = 0;

void initPatterCreator(uint16_t seed, uint16_t ledCount)
{
	initAPA102(ledCount);
	srand(seed);
}

/*
   fills the provided colour_t array with a continous sequence of
   changing colours
Param: *dest -> a colour_t array to fill with colours of the rainbow
 */
void createRainbow(colour_t *dest)
{
	uint8_t ran = rand() >> 4; // only interresting in top 4 bits
	for(uint16_t i = 0; i < getNumberOfLEDs(); i++)
	{
		uint8_t p = ran - i * 8;
		dest[i] = hsvToRgb((uint32_t)p * 359 / 256, 255, 255);
	}

	loopCount++;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// The following functions have been ported from: 
// https://github.com/pololu/apa102-arduino to work with AVRs
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

static void brightTwinkleColourAdjust(uint8_t *colour)
{
	if (*colour == 255)
	{
		// if reached max brightness, set to an even value to start fade
		*colour = 254;
	}
	else if (*colour % 2)
	{
		// if odd, approximately double the brightness
		// you should only use odd values that are of the form 2^n-1,
		// which then gets a new value of 2^(n+1)-1
		// using other odd values will break things
		*colour = *colour * 2 + 1;
	}
	else if (*colour > 0)
	{
		fade(colour, 4);
		if (*colour % 2)
		{
			(*colour)--;  // if faded colour is odd, subtract one to keep it even
		}
	}
}

static void colourExplosionColourAdjust(uint8_t *colour, uint8_t propChance,
		uint8_t *leftColour, uint8_t *rightColour)
{
	if (*colour == 31 && (rand() % 10 + 1)!= 0)
	{
		if (leftColour != 0 && *leftColour == 0)
		{
			*leftColour = 1;  // if left LED exists and colour is zero, propagate
		}
		if (rightColour != 0 && *rightColour == 0)
		{
			*rightColour = 1;  // if right LED exists and colour is zero, propagate
		}
	}
	brightTwinkleColourAdjust(colour);
}

void createExplosion(colour_t *cols, uint8_t noNewBursts)
{
	// adjust the cols of the first LED
	colourExplosionColourAdjust(&cols[0].red, 9, (uint8_t*)0, &cols[1].red);
	colourExplosionColourAdjust(&cols[0].green, 9, (uint8_t*)0, &cols[1].green);
	colourExplosionColourAdjust(&cols[0].blue, 9, (uint8_t*)0, &cols[1].blue);

	for (int i = 1; i < getNumberOfLEDs() - 1; i++)
	{
		// adjust the cols of second through second-to-last LEDs
		colourExplosionColourAdjust(&cols[i].red, 9, &cols[i-1].red, &cols[i+1].red);
		colourExplosionColourAdjust(&cols[i].green, 9, &cols[i-1].green, &cols[i+1].green);
		colourExplosionColourAdjust(&cols[i].blue, 9, &cols[i-1].blue, &cols[i+1].blue);
	}

	// adjust the cols of the last LED
	colourExplosionColourAdjust(&cols[getNumberOfLEDs()-1].red, 9, &cols[getNumberOfLEDs()-2].red, (uint8_t*)0);
	colourExplosionColourAdjust(&cols[getNumberOfLEDs()-1].green, 9, &cols[getNumberOfLEDs()-2].green, (uint8_t*)0);
	colourExplosionColourAdjust(&cols[getNumberOfLEDs()-1].blue, 9, &cols[getNumberOfLEDs()-2].blue, (uint8_t*)0);

	if (!noNewBursts)
	{
		// if we are generating new bursts, randomly pick one new LED
		// to light up
		for (int i = 0; i < 1; i++)
		{
			//      int j = random(getNumberOfLEDs());  // randomly pick an LED
			int j = rand() % getNumberOfLEDs() + 1;  // randomly pick an LED

			switch(rand() % 7 + 1)  // randomly pick a colour
			{
				// 2/7 chance we will spawn a red burst here (if LED has no red component)
				case 0:
				case 1:
					if (cols[j].red == 0)
					{
						cols[j].red = 1;
					}
					break;

					// 2/7 chance we will spawn a green burst here (if LED has no green component)
				case 2:
				case 3:
					if (cols[j].green == 0)
					{
						cols[j].green = 1;
					}
					break;

					// 2/7 chance we will spawn a white burst here (if LED is all off)
				case 4:
				case 5:
					/*if ((cols[j].red == 0) && (cols[j].green == 0) && (cols[j].blue == 0))
					  {
					  colour_t col;
					  col.red = 1; col.green = 1; col.blue = 1;
					  cols[j] = col;
					  }*/
					break;

					// 1/7 chance we will spawn a blue burst here (if LED has no blue component)
				case 6:
					if (cols[j].blue == 0)
					{
						cols[j].blue = 1;
					}
					break;

				default:
					break;
			}
			cols[j].brightness = 15;
		}
	}

	loopCount++;
}

static void fade(uint8_t *val, uint8_t fadeTime)
{
	if (*val != 0)
	{
		uint8_t subAmt = *val >> fadeTime;  // val * 2^-fadeTime
		if (subAmt < 1)
			subAmt = 1;  // make sure we always decrease by at least 1
		*val -= subAmt;  // decrease value of byte pointed to by val
	}
}

void createChristmas(colour_t *cols)
{
	// loop counts to leave strip initially dark
	const uint8_t initialDarkCycles = 10;
	// loop counts it takes to go from full off to fully bright
	const uint8_t brighteningCycles = 20;

	// if getNumberOfLEDs() is not an exact multiple of our repeating pattern size,
	// it will not wrap around properly, so we pick the closest LED count
	// that is an exact multiple of the pattern period (20) and is not smaller
	// than the actual LED count.
	//uint16_t extendedLEDCount = (((getNumberOfLEDs()-1)/20)+1)*20;
	uint16_t extendedLEDCount = (((getNumberOfLEDs()-1)/20)+1)*20;

	for (int i = 0; i < extendedLEDCount; i++)
	{
		uint8_t brightness = (loopCount - initialDarkCycles)%brighteningCycles + 1;
		uint8_t cycle = (loopCount - initialDarkCycles)/brighteningCycles;

		// transform i into a moving idx space that translates one step per
		// brightening cycle and wraps around
		uint16_t idx = (i + cycle)%extendedLEDCount;
		if (idx < getNumberOfLEDs())  // if our transformed index exists
		{
			if (i % 4 == 0)
			{
				// if this is an LED that we are colouring, set the colour based
				// on the LED and the brightness based on where we are in the
				// brightening cycle
				switch ((i/4)%5)
				{
					case 0:  // red
						cols[idx].red = 200 * brightness/brighteningCycles;
						cols[idx].green = 10 * brightness/brighteningCycles;
						cols[idx].blue = 10 * brightness/brighteningCycles;
						break;
					case 1:  // green
						cols[idx].red = 10 * brightness/brighteningCycles;
						cols[idx].green = 200 * brightness/brighteningCycles;
						cols[idx].blue = 10 * brightness/brighteningCycles;
						break;
					case 2:  // orange
						cols[idx].red = 200 * brightness/brighteningCycles;
						cols[idx].green = 120 * brightness/brighteningCycles;
						cols[idx].blue = 0 * brightness/brighteningCycles;
						break;
					case 3:  // blue
						cols[idx].red = 10 * brightness/brighteningCycles;
						cols[idx].green = 10 * brightness/brighteningCycles;
						cols[idx].blue = 200 * brightness/brighteningCycles;
						break;
					case 4:  // magenta
						cols[idx].red = 200 * brightness/brighteningCycles;
						cols[idx].green = 64 * brightness/brighteningCycles;
						cols[idx].blue = 145 * brightness/brighteningCycles;
						break;
				}

				cols[idx].brightness = 15;
			}
			else
			{
				// fade the 3/4 of LEDs that we are not currently brightening
				fade(&cols[idx].red, 3);
				fade(&cols[idx].green, 3);
				fade(&cols[idx].blue, 3);
			}
		}
	}

	loopCount++;
}
