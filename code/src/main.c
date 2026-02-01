#include "drivers.h"

// **Note** pico/stdlib.h IS needed despite clangd saying otherwise
#include "pico/stdlib.h"     
#include "pico/multicore.h"
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <sys/_intsup.h>

#include "constants.h"
#include <stdbool.h>
#include "data_structs.h"

#include "system_state_loop.h"

// Needed in glcd
void delay_ms(unsigned int ms) {
    sleep_ms(ms);
}

// Second core will watch for button inputs and update display
void core1_entry() {
    while (true) {      
        run_system_state_loop_core1();

        sleep_ms(SYSTEM_DELAY_MS);
    }
}

int main() {

    // Set up pins, SPI, I2C, etc
    board_setup();

    // Ensure all protocols had time to start up
    sleep_ms(SYSTEM_DELAY_MS);

    // Run the display and buttons on core 1
    multicore_launch_core1(core1_entry);

    // Show the boot screen
    sleep_ms(500);

    while (true) {      
        // Run everything else on core 0
        run_system_state_loop_core0();

        sleep_ms(SYSTEM_DELAY_MS);
    }
}

