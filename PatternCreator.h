#ifndef GUARD_PATTER_CREATOR_H
#define GUARD_PATTER_CREATOR_H

#include <stdint.h>
#include "APA102.h" // colour_t

void initPatterGenerator(uint16_t, uint16_t);
void createRainbow(colour_t *, uint16_t);
void fade(uint8_t*, uint8_t);
void christmasColors(colour_t *);

void colourExplosionColourAdjust(uint8_t *, uint8_t, uint8_t *, uint8_t *);
void brightTwinkleColourAdjust(uint8_t);
void colourExplosion(colour_t *, uint8_t);

#endif
