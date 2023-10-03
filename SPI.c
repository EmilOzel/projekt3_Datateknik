#include "SPI.h"

void SPI_MasterInit(void) {
    // Set MOSI, SCK, and SS as output
    DDRB |= (1<<DDB2)|(1<<DDB1)|(1<<DDB0);

    // Enable SPI, set as Master, set CPOL=1 and CPHA=0, set clock rate to achieve 250000 Hz
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(0<<CPHA)|(1<<SPR1)|(1<<SPR0);
}

uint16_t SPI_ReadTemperature(void) {
    // Pull SS low to start transmission
    PORTB &= ~(1<<PORTB2);

    // Transmit and receive MSB
    SPDR = 0x00;
    while (!(SPSR & (1<<SPIF)));
    uint8_t msb = SPDR;

    // Transmit and receive LSB
    SPDR = 0x00;
    while (!(SPSR & (1<<SPIF)));
    uint8_t lsb = SPDR;

    // Pull SS high to end transmission
    PORTB |= (1<<PORTB2);

    // Combine MSB and LSB to form 16-bit data
    return ((uint16_t)msb << 8) | lsb;
}
