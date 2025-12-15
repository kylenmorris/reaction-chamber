#include <hardware/gpio.h>
#include "pico/stdlib.h"     // **Note** pico/stdlib.h IS needed despite clangd saying otherwise
#include "pico/multicore.h"
#include <stdio.h>
#include <stdbool.h>

#include "constants.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"

#include "system_state_loop.h"
#include "pico_led.h"

void core1_entry() {
    while (true) {
        button_ctrl_step(); 
        display_ctrl_draw();
        sleep_ms(SYSTEM_DELAY_MS);
    }
}

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

    while (true) {      

        run_system_state_loop();
        sleep_ms(SYSTEM_DELAY_MS);

    }
}

