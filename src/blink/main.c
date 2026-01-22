#include <hardware/gpio.h>
#include "board_setup.h"
// **Note** pico/stdlib.h IS needed despite clangd saying otherwise
#include "pico/stdlib.h"     
#include "pico/multicore.h"

#include <hardware/spi.h>
#include <stdbool.h>
#include <sys/_intsup.h>

#include "constants.h"

// Needed in glcd
void delay_ms(unsigned int ms) {
    sleep_ms(ms);
}

// Second core will watch for button inputs and update display
void core1_entry() {
    while (true) {      

        run_system_state_loop_core1();
        sleep_ms(SYSTEM_DELAY_MS);

    }}

// Main entry point and loop, though this is really just startup. 
// Most logic is in system_state_loop.c
int main() {

    board_setup();

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    pico_set_led(HIGH); // Startup LED

    multicore_launch_core1(core1_entry);

    sleep_ms(250);
    pico_set_led(LOW); // Finished startup

    delay_ms(1000);

    while (true) {      

        run_system_state_loop_core0();
        sleep_ms(SYSTEM_DELAY_MS);

    }
}

