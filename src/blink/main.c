#include <hardware/gpio.h>
#include "fonts/font5x7.h"
#include "pico/stdlib.h"     // **Note** pico/stdlib.h IS needed despite clangd saying otherwise
#include "pico/multicore.h"
// #include <stdio.h>
#include <hardware/spi.h>
#include <stdbool.h>
#include <sys/_intsup.h>

#include "constants.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"

#include "system_state_loop.h"
#include "pico_led.h"
#include "screens.h"

#include "glcd.h"

// Needed in glcd
void delay_ms(unsigned int ms) {
    sleep_ms(ms);
}

// Second core will watch for button inputs and update display
void core1_entry() {
    while (true) {
        button_ctrl_step(); 
        display_ctrl_draw();
        sleep_ms(SYSTEM_DELAY_MS);
    }
}


void init_spi1() {
    gpio_init(SPI1_CS0_PIN);
    gpio_set_dir(SPI1_CS0_PIN, GPIO_OUT);
    gpio_put(SPI1_CS0_PIN, HIGH); // Deselect

    spi_init(spi1, SPI_BAUDRATE_DISPLAY);       // keeping 4 MHz for now
    
    gpio_set_function(SPI1_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI1_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SPI);

    gpio_set_function(SPI1_CS0_PIN, GPIO_FUNC_SPI);  // init sd card csn

	sleep_ms(1);
}

// Main entry point and loop, though this is really just startup. 
// Most logic is in system_state_loop.c
int main() {

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    pico_set_led(HIGH); // Startup LED

    stdio_init_all();

    button_ctrl_init();
    temp_sens_ctrl_init();
    heater_ctrl_init();
    tube_optical_ctrl_init();

    multicore_launch_core1(core1_entry);

    sleep_ms(500);
    pico_set_led(LOW); // Finished startup

    /* Currently glcd initializes the relevant GPIO pins and SPI internally.
       I'd like to move it outside eventually, but note spi0 is initialized here. */
    glcd_init();

    init_spi1();

    
    glcd_clear_buffer();
    glcd_tiny_draw_string(0, 0, "PathoScan: Booting...");
    glcd_write();

    delay_ms(1000);

    while (true) {      

        run_system_state_loop();
        sleep_ms(SYSTEM_DELAY_MS);

    }
}

