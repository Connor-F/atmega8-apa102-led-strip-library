#ifndef GUARD_APA102_H
#define GUARD_APA102_H

#include <stdint.h>

// represents a 24 bit RGB colour for an LED, including
// a brightness (0-31 range with 31 being max brightness)
typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t brightness;
} colour_t;

// number of LEDs in the LED strip being controlled
uint16_t numberOfLEDs;

void initAPA102(uint16_t);

void apa102LightLEDs(colour_t *);
colour_t hsvToRgb(uint16_t, uint8_t, uint8_t);

uint16_t getNumberOfLEDs(void);

#endif
