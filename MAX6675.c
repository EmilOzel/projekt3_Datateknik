#include "MAX6675.h"
#include "SPI.h"
#include <util/delay.h>

uint16_t readMAX6675(void) {
    uint16_t data = 0;

    PORTB &= ~(1<<PB0); // Pull SS low to start communication
    _delay_ms(1); // Wait for data to be ready

    data = SPI_ReadTemperature(); // Read 16-bit data from MAX6675

    PORTB |= (1<<PB0); // Pull SS high to end communication

    return data >> 3; // Shift 3 bits to get the 12-bit temperature data
}
