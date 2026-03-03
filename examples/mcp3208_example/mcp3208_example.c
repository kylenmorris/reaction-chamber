#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi1
#define PIN_MISO 12
#define PIN_CS   13
#define PIN_SCK  14
#define PIN_MOSI 15

// Function to read a specific channel from the MCP3208
uint16_t read_mcp3208(uint8_t channel) {
    // Ensure we only use valid channels (0-7)
    if (channel > 7) return 0;

    uint8_t tx_buf[3];
    uint8_t rx_buf[3];

    // Byte 0: 0 0 0 0 0 Start(1) SGL(1) D2
    tx_buf[0] = 0x06 | ((channel >> 2) & 0x01); 
    
    // Byte 1: D1 D0 0 0 0 0 0 0
    tx_buf[1] = (channel & 0x03) << 6;          
    
    // Byte 2: Dummy byte to clock out the remaining data
    tx_buf[2] = 0x00;                           

    // Pull Chip Select low to initiate communication
    gpio_put(PIN_CS, 0);

    // Write the 3 bytes while simultaneously reading the response
    spi_write_read_blocking(SPI_PORT, tx_buf, rx_buf, 3);

    // Push Chip Select high to end communication
    gpio_put(PIN_CS, 1);

    // Construct the 12-bit result from the response bytes
    // The first RX byte is ignored. 
    // The lower 4 bits of the second RX byte contain the upper 4 bits of the result.
    // The third RX byte contains the lower 8 bits of the result.
    uint16_t result = ((rx_buf[1] & 0x0F) << 8) | rx_buf[2];

    return result;
}

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Initialize the Chip Select pin as standard GPIO output
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1); // Set high (inactive) by default
    
    // Give the user a moment to connect via USB serial monitor
    sleep_ms(2000);
    printf("MCP3208 Initialization Complete.\n");

    while(true) {
        // Read Channel 0
        uint16_t adc_value = read_mcp3208(0);
        
        // Calculate voltage (Assuming VREF = 3.3V on the MCP3208)
        float voltage = (adc_value / 4095.0f) * 3.3f;

        // Print to the console
        printf("Channel 0: %04d (%.2f V)\n", adc_value, voltage);

        // Wait half a second before reading again
        sleep_ms(1000);
    }
}