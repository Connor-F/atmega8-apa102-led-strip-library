#include "APA102.h"
#include "SPI.h"

/*
   prepares the apa102 led strip for use (sets up spi)
	Param: leds -> the number of leds in the strip that is being controlled
*/
void initAPA102(uint16_t leds)
{
	numberOfLEDs = leds;
	initSPI();
}

/*
   sends the initialisation signal to the led strip
*/
static void sendStartFrame(void)
{
	spiExchangeByte(0x00);
	spiExchangeByte(0x00);
	spiExchangeByte(0x00);
	spiExchangeByte(0x00);
}

/*
   sends the end signal to the led strip
*/
static void sendEndFrame(void)
{
	spiExchangeByte(0xff);
	spiExchangeByte(0xff);
	spiExchangeByte(0xff);
	spiExchangeByte(0xff);
}

/*
   lights len number of LEDs in the strip using the colours provided
	Param: *col -> an array of colour_t where each element represents 1 LED in the strip
		   len -> the length of the colour array (also reperesents the number of LEDs that will be illuminated)
	Returns: 0 on success
			 1 if the length provided was > the number of LEDs in the strip
*/
uint8_t apa102LightLEDs(colour_t *col, uint16_t len)
{
	if(len > numberOfLEDs)
		return 1;

	// start frame indicator
	sendStartFrame();

	// led frames
	for(uint16_t i = 0; i < len; i++)
	{
		if(col[i].brightness > 31) // default to 15 (50%) brightness if brightness value was invalid
			spiExchangeByte(0xe0 + 15);
		else
			spiExchangeByte(0xe0 + col[i].brightness);
				
		spiExchangeByte(col[i].blue);
		spiExchangeByte(col[i].green);
		spiExchangeByte(col[i].red);
	}

	// end frame indicator
	sendEndFrame();

	return 0;
}

/* Converts a color from HSV to RGB. Used as the HSV colour space is allows for
   easier generation of colours by adjusting the hue value only
	Param: hue -> a number between 0 and 360.
	       saturation -> a number between 0 and 255
	       value -> a number between 0 and 255
	Returns: a colour_t containing rgb values from the provided hsv values
*/
colour_t hsvToRgb(uint16_t hue, uint8_t saturation, uint8_t value)
{
    uint8_t f = (hue % 60) * 255 / 60;
    uint8_t p = (255 - saturation) * (uint16_t)value / 255;
    uint8_t q = (255 - f * (uint16_t)saturation / 255) * (uint16_t)value / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)saturation / 255) * (uint16_t)value / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((hue / 60) % 6)
	{
        case 0: r = value; g = t; b = p; break;
        case 1: r = q; g = value; b = p; break;
        case 2: r = p; g = value; b = t; break;
        case 3: r = p; g = q; b = value; break;
        case 4: r = t; g = p; b = value; break;
        case 5: r = value; g = p; b = q; break;
    }

	colour_t col;
	col.red = r;
	col.green = g;
	col.blue = b;
	col.brightness = 7;
	return col;
}
