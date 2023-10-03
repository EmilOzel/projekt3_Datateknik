#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void SPI_MasterInit(void);
uint16_t SPI_ReadTemperature(void);

#endif // SPI_H
