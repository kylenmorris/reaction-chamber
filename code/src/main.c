#include "drivers.h"

// **Note** pico/stdlib.h IS needed despite clangd saying otherwise
#include "pico/stdlib.h"     
#include "pico/multicore.h"
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <stdio.h>
#include <sys/_intsup.h>

#include "constants.h"
#include <stdbool.h>

#include "system_state_loop.h"

// Needed for glcd
void delay_ms(unsigned int ms) {
    sleep_ms(ms);
}

// Second core will update display
void core1_entry() {
    while (true) {      
        run_system_state_loop_core1();

        sleep_ms(SYSTEM_DELAY_MS);
    }
}

int main() {

    // Set up pins, SPI, I2C, etc
    board_setup();

    printf("System booting...\n");

    sleep_ms(SYSTEM_DELAY_MS * 5);

    // Hold up and down to enter debug
    // if (!hw_button_get_raw(BUTTON_PINS[UP])
    //     && !hw_button_get_raw(BUTTON_PINS[DOWN])) {
    //         DEBUG_MODE = true; // enable some menu items
    //         // populate file list
    //         load_all_sd_filenames_into_global();
            
    //         // delete all files
    //         for (int i = 0; i < MAX_FILES; i++) {
    //             if (results_menu_items[i][0] != '\0') {
    //                 delete_file_from_sd_card(results_menu_items[i]);
    //             }
    //         }

    //         printf("Entered debug mode, deleted all files from SD card\n");
    // }

    // if (DEBUG_MODE) {
    //     gIdleMenuIM.
    // }

    // Run the display on core 1
    multicore_launch_core1(core1_entry);

    // Show the boot screen
    // sleep_ms(200);

    while (true) {      
        run_system_state_loop_core0();

        sleep_ms(SYSTEM_DELAY_MS);
    }
}

