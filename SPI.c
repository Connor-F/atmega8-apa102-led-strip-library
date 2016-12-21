#include "SPI.h"

/*
   sets up the spi bus, enables output/input pins, sets up prescaler
*/
void initSPI(void) 
{
	SPI_SS_DDR |= (1 << SPI_SS);                        /* set SS output */
	SPI_SS_PORT |= (1 << SPI_SS);       /* start off not selected (high) */

	SPI_MOSI_DDR |= (1 << SPI_MOSI);                   /* output on MOSI */
	SPI_MISO_PORT |= (1 << SPI_MISO);                  /* pullup on MISO */
	SPI_SCK_DDR |= (1 << SPI_SCK);                      /* output on SCK */

	SPCR |= (1 << SPR1);                /* div 16, safer for breadboards */
	SPCR |= (1 << MSTR);                                  /* clockmaster */
	SPCR |= (1 << SPE);                                        /* enable */
}

/*
   exchanges a byte from the master to the slave (and therefore receives a
   byte from the slave)
	Param: byte -> the value to send to the slave
	Returns: the byte received from the slave
*/
uint8_t spiExchangeByte(uint8_t byte) 
{
	SPDR = byte;                       /* SPI starts sending immediately */
	loop_until_bit_is_set(SPSR, SPIF);                /* wait until done */
	return SPDR;
}
