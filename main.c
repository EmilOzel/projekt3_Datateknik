#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <I2C.h>
#include <ssd1306.h>
#include <SPI.h>
#include <MAX6675.h>

uint16_t customSPI_Transmit() {
    // Pull SS low to start transmission//
    PORTB &= ~(1 << PORTB2);

    // Transmit and receive MSB//
    SPDR = 0x00;
    while (!(SPSR & (1 << SPIF)));
    uint8_t msb = SPDR;

    // Transmit and receive LSB//
    SPDR = 0x00;
    while (!(SPSR & (1 << SPIF)));
    uint8_t lsb = SPDR;

    // Pull SS high to end transmission//
    PORTB |= (1 << PORTB2);

    // Combine MSB and LSB to form 16-bit data//
    return ((uint16_t)msb << 8) | lsb;
}

int main(void) {
    _i2c_address = 0x78;

    SPI_MasterInit();
    I2C_Init();
    InitializeDisplay();
    clear_display();

    while (1) {
        uint16_t data = customSPI_Transmit();

        // Check for open circuit or fault//
        if (data & 0x0001) {
            clear_display();
            sendStrXY("Error", 2, 2);
            _delay_ms(500);
            continue;
        }

        float temperature = (data >> 3) * 0.25;  // Shift right 3 bits to get 12-bit data and convert to temperature//

        // Manually convert float to string with two decimal places for OLED//
        int intPart = (int)temperature;
        int decimalPart = (int)((temperature - intPart) * 100);
        char oledBuffer[10];
        snprintf(oledBuffer, sizeof(oledBuffer), "%d.%02d", intPart, decimalPart);
        clear_display();

        // Print temperature to OLED display//
        sendStrXY(oledBuffer, 2, 2);

        _delay_ms(500);
    }

    return 0;
}
