#include "board_setup.h"

#include "constants.h"
#include <hardware/gpio.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <stdbool.h>

#include "glcd.h"

void init_spi1() {
    spi_init(spi1, SPI_BAUDRATE_DISPLAY);       // keeping 4 MHz for now
    
    gpio_set_function(SPI1_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI1_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SPI);

	sleep_ms(1);
}

void init_spi0() {
    gpio_init(SPI0_CS0_PIN);
    gpio_set_dir(SPI0_CS0_PIN, GPIO_OUT);
    gpio_put(SPI0_CS0_PIN, HIGH); // Deselect

    spi_init(spi0, SPI_BAUDRATE_DISPLAY);       // keeping 4 MHz for now
    
    gpio_set_function(SPI0_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_MOSI_PIN, GPIO_FUNC_SPI);

    gpio_set_function(SPI0_CS0_PIN, GPIO_FUNC_SPI);  // init sd card csn

    sleep_ms(1);
}

static void init_gpio_as_button(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    gpio_pull_up(gpio_pin);
}

void board_setup(void) {
    
    // spi0 and spi1
    init_spi0();
    init_spi1();

    // Buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
        init_gpio_as_button(BUTTON_PINS[i]);
    }

    // Heater


    // Display
    gpio_init(SPI0_CS0_PIN);
    gpio_set_dir(SPI0_CS0_PIN, GPIO_OUT);
    gpio_put(SPI0_CS0_PIN, HIGH); // Deselect

    gpio_set_function(SPI1_CS0_PIN, GPIO_FUNC_SPI);  // init sd card csn

    glcd_init();

    // Photodiodes

    
    // Temperature sensors


    // Tube switches
    

    // SD card reader
    gpio_init(SPI1_CS0_PIN);
    gpio_set_dir(SPI1_CS0_PIN, GPIO_OUT);
    gpio_put(SPI1_CS0_PIN, HIGH); // Deselect


}