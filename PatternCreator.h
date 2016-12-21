#ifndef GUARD_PATTER_CREATOR_H
#define GUARD_PATTER_CREATOR_H

#include <stdint.h>
#include "APA102.h" // colour_t, initAPA102

void initPatterCreator(uint16_t, uint16_t);
void createRainbow(colour_t *);
void createChristmas(colour_t *);
void createExplosion(colour_t *, uint8_t);

static void colourExplosionColourAdjust(uint8_t *, uint8_t, uint8_t *, uint8_t *);
static void brightTwinkleColourAdjust(uint8_t *);
static void fade(uint8_t*, uint8_t);

#endif
