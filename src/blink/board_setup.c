#include "board_setup.h"

#include "constants.h"
#include <hardware/gpio.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <stdbool.h>

#include "glcd.h"
#include <hardware/gpio.h>
#include <stdbool.h>

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

static void init_gpio_as_button(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    gpio_pull_up(gpio_pin);
    }

static void init_gpio_as_csn(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, HIGH); // Deselect

    gpio_set_function(gpio_pin, GPIO_FUNC_SPI);  // init sd card csn
}

// Perform initialisation
int init_led(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

void init_spi1() {
    spi_init(spi1, SPI_BAUDRATE_DISPLAY);       // keeping 4 MHz for now
    
    gpio_set_function(SPI1_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI1_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SPI);
}

void init_spi0() {
    spi_init(spi0, SPI_BAUDRATE_DISPLAY);       // keeping 4 MHz for now
    
    gpio_set_function(SPI0_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_MOSI_PIN, GPIO_FUNC_SPI);
}

void board_setup(void) {

    stdio_init_all();
    init_led();

    // spi0 and spi1
    init_spi0();
    init_spi1();

    // Buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
        init_gpio_as_button(BUTTON_PINS[i]);
    }

    // Heater


    // Display
    init_gpio_as_csn(SPI0_CS0_PIN);

    // Photodiodes

    
    // Temperature sensors


    // Tube switches
    

    // SD card reader
    init_gpio_as_csn(SPI1_CS0_PIN);

    // Libraries
    glcd_init();

}