#include "drivers.h"

#include "constants.h"
#include <stdbool.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/i2c.h>

#include "ff.h"
#include "glcd.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

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

static void init_gpio_as_button(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    gpio_pull_up(gpio_pin);
}

void board_setup(void) {

    #ifdef USE_HW_PICO

        stdio_init_all();
        init_led();

        // Spi 0
        spi_init(spi0, SPI0_BAUDRATE);     
        
        gpio_set_function(SPI0_SCK_PIN, GPIO_FUNC_SPI);
        // gpio_set_function(SPI0_MISO_PIN, GPIO_FUNC_SPI);
        gpio_set_function(SPI0_MOSI_PIN, GPIO_FUNC_SPI);

        // Spi 1
        spi_init(spi1, SPI1_BAUDRATE);       
        
        gpio_set_function(SPI1_SCK_PIN, GPIO_FUNC_SPI);
        // gpio_pull_down(SPI1_MISO_PIN); // Or gpio_pull_up(SPI1_MISO_PIN);
        gpio_set_function(SPI1_MISO_PIN, GPIO_FUNC_SPI);
        gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SPI);

        // Buttons
        for (int i = 0; i < NUM_BUTTONS; i++) {
            init_gpio_as_button(BUTTON_PINS[i]);
        }

        // Heater
        gpio_init(HEATER_PIN);
        gpio_set_dir(HEATER_PIN, GPIO_OUT);
        gpio_put(HEATER_PIN, 0); // make sure heater is off to start

        // Display
        gpio_init(SPI0_CSn_DISPLAY_PIN);
        gpio_set_dir(SPI0_CSn_DISPLAY_PIN, GPIO_OUT);
        gpio_put(SPI0_CSn_DISPLAY_PIN, HIGH); // Deselect

        gpio_set_function(SPI0_CSn_DISPLAY_PIN, GPIO_FUNC_SPI); 


        // Photodiodes

        
        // Temperature sensors
        i2c_init(i2c0, I2C_BAUDRATE);
        gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(I2C_SDA_PIN);
        gpio_pull_up(I2C_SCL_PIN);

        // Tube switches
        gpio_init(SPI1_CS_SHIFT_SWITCHES_SR_PIN_0);
        gpio_set_dir(SPI1_CS_SHIFT_SWITCHES_SR_PIN_0, GPIO_OUT);
        gpio_put(SPI1_CS_SHIFT_SWITCHES_SR_PIN_0, 1);
        
        gpio_init(SPI1_CS_SHIFT_SWITCHES_SR_PIN_1);
        gpio_set_dir(SPI1_CS_SHIFT_SWITCHES_SR_PIN_1, GPIO_OUT);
        gpio_put(SPI1_CS_SHIFT_SWITCHES_SR_PIN_1, 1);

        // SD card reader
        // gpio_init(SPI1_CS0_PIN);
        // gpio_set_dir(SPI1_CS0_PIN, GPIO_OUT);
        // gpio_put(SPI1_CS0_PIN, HIGH); // Deselect
        // We don't init the pin as a spi pin so we can easily control multiple spi devices... according to chat

        // Debug pin
        gpio_init(PROBE_PIN);
        gpio_set_dir(PROBE_PIN, GPIO_IN);

        // Libraries
        glcd_init();


    #else

    #endif

}