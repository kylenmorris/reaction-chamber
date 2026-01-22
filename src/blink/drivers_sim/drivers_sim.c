#include "button_interface.h"
#include "heater_interface.h"
#include "temp_sens_interface.h"
#include "tube_optical_interface.h"
#include "tube_sens_interface.h"

#include "constants.h"
#include "led_interface.h"
#include <hardware/gpio.h>

// Turn the led on or off
void pico_set_led(bool led_on) {

}

// Checks for button presses and updates gButtonInput if a button was pressed
int hw_button_get_raw(int button_index) {

}


void hw_heater_toggle(bool enable) {

}


float hw_read_temperature_sensor(int sensor_id) {
    // if (gTempStatus.chamber_temp < TEMP_LOW_C + 1.5) {
    //     return gTempStatus.chamber_temp + 0.1f;
    // } else {
    //     return gTempStatus.chamber_temp;
    // }
}


uint16_t hw_adc_read_raw(int adc_index, int channel) {
    // for (int i = 0; i < NUM_TUBES - 6; i++) {
                                                                              
    //     gOpticalInputs.intensity[i] = (OPTICAL_REACTION_THRESHOLD + 100 - rand() % 100000);
    // }
    // for (int i = NUM_TUBES - 6; i < NUM_TUBES; i++) {
    //     gOpticalInputs.intensity[i] = rand() % 3000;
    // }
}


void hw_led_set(bool led_on) {

}


uint16_t hw_tube_sens_read_all(void) {

    // // Handle debug button press to insert new random tube
    // if (button_is_pressed(DEBUG)) {

    //     // printf("Debug button pressed, inserting tube\n");
    //     // Find an unused tube
    //     int available_tubes[NUM_TUBES];
    //     int available_count = 0;
        
    //     for (int i = 0; i < NUM_TUBES; i++) {
    //         if (!used_tubes[i]) {
    //             available_tubes[available_count++] = i;
    //         }
    //     }
        
    //     // If tubes are available, pick a random one
    //     if (available_count > 0) {
    //         int random_idx = rand() % available_count;
    //         int tube_index = available_tubes[random_idx];

    //         // printf("Inserting tube %d\n", tube_index);
            
    //         gSysControl.tube_present[tube_index] = true;
    //         used_tubes[tube_index] = true;
    //     }
    // }
}
