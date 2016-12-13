#ifndef GUARD_SPI_H
#define GUARD_SPI_H

#include <avr/io.h>
#include <stdint.h>

#define SPI_SS_DDR DDRB
#define SPI_SS_PORT PORTB
#define SPI_MOSI_DDR DDRB
#define SPI_MOSI_PORT PORTB
#define SPI_MISO_PORT PORTB
#define SPI_SCK_DDR DDRB
#define SPI_SS PB2
#define SPI_MOSI PB3
#define SPI_MISO PB4
#define SPI_SCK PB5

#define SLAVE_SELECT    SPI_SS_PORT &= ~(1 << SPI_SS)
#define SLAVE_DESELECT  SPI_SS_PORT |= (1 << SPI_SS)

void initSPI(void);
uint8_t spiExchangeByte(uint8_t);

#endif
