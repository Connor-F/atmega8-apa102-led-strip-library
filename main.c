#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h> // memmove

#include "ESP8266.h"
#include "PatternCreator.h"

#define NUMBER_OF_LEDS 60

#define CMD_NONE 0x00 // the default state for initial command (no leds on)
#define CMD_RAINBOW 0x72 // 'r'
#define CMD_EXPLOSION 0x65 // 'e'
#define CMD_CHRISTMAS 0x63 // 'c'

bool wasColonCharacter = false; // used to parse data from esp8266 messages
volatile uint8_t currentCommand = CMD_NONE; // which led setting the user chose

/*
   triggered when the uart module receives data. Parses the data
   for the command receieved 
*/
ISR(USART_RX_vect)
{
	// msgs from the esp8266 arrive in the format 
	// +IPD,<ID>,<len>[,<remote IP>,<remote port>]:<data>
	// so we skip straight to the colon to read the data
	if(wasColonCharacter) // colon was previous character, so next char must be the command
	{
		currentCommand = uartGetByte();
		wasColonCharacter = false;
	}
	else // check if we rx a colon, if so we know the next char will be the command
	{
		if(uartGetByte() == ':')
			wasColonCharacter = true;
	}
}

/*
   enables interrupts and sets up neccessary modules
*/
void initialise(void)
{
	UCSR0B |= 1 << RXCIE0; // enable uart rx interrupts
	sei(); // enable global interrupts

	initPatterCreator(53, NUMBER_OF_LEDS);
	initServerEsp();
}

int main(void)
{
	initialise();

	colour_t cols[NUMBER_OF_LEDS]; // the colour pattern that will be displayed on the led strip
	bool rainbowExists = false; // monitors whether the rainbow pattern has alread been created,
	// if it has then we shuffle the colour array 1 place to the right to create a moving rainbow
	// effect

	while(1)
	{
		uint8_t updateDelay = 20; // rainbow effect requires longer delays to look smooth
		if(currentCommand == CMD_RAINBOW)
		{   
			if(!rainbowExists)
				createRainbow(cols);

			updateDelay = 110;
			// shift elements by 1 place right (moving effect)
			colour_t temp = cols[NUMBER_OF_LEDS - 1];
			memmove(cols + 1, cols, sizeof(cols) - sizeof(cols[0]))    ;
			cols[0] = temp;
			rainbowExists = true;
		}
		else if(currentCommand == CMD_EXPLOSION)
		{
			rainbowExists = false;
			createExplosion(cols, 0);
		}
		else if(currentCommand == CMD_CHRISTMAS)
		{
			rainbowExists = false;
			createChristmas(cols);
		}
		else if(currentCommand == CMD_NONE) // we wait for a user command
			continue;

		apa102LightLEDs(cols); 
		for(uint8_t i = 0; i < updateDelay; i++)
			_delay_ms(1);
	}

	return 0;
}
