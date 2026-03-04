#include "drivers.h"
#include "data_structs.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

#define random rand

#ifndef USE_HW_PICO


// Turn the led on or off
void pico_set_led(bool led_on) {
    printf("LED %s\n", led_on ? "ON" : "OFF");
}

uint32_t get_current_time(void) {
    return gSimTime;
}

#endif

#ifndef USE_HW_BUTTONS
int hw_button_get_raw(int button_gpio) {
    return 1; // no presses
}
#endif

#ifndef USE_HW_HEATER
void hw_heater_toggle(bool enable) {
    // printf("Heater %s\n", enable ? "ON" : "OFF");
    gHeaterState.heaterOn = enable;
}
#endif

#ifndef USE_HW_TEMP_SENS
float hw_read_temperature_sensor(int sensor_id) {

    if (gSimParams.force_temp_sensor_fault) return 211.0f;

    // if heater is on, temp goes up
    if (gHeaterState.heaterOn == true) {
        gTempStatus.chamber_temp += gSimParams.heat_rate;
    } else {
        if (gTempStatus.chamber_temp > gSimParams.ambient_temp) {
            gTempStatus.chamber_temp -= gSimParams.cool_rate;
        }
    }

    return gTempStatus.chamber_temp;
}
#endif

#ifndef USE_HW_OPTICAL
uint16_t hw_adc_read_raw(int adc_index, int channel) {
    
    for (int i = 0; i < NUM_TUBES - 6; i++) {                                                                      
        gOpticalInputs.intensity[i] = (OPTICAL_REACTION_THRESHOLD + 100);
    }
    for (int i = NUM_TUBES - 6; i < NUM_TUBES; i++) {
        gOpticalInputs.intensity[i] = 200;
    }
    return 0;
}

#endif

// Track which tubes have been inserted via debug button
static bool used_tubes[NUM_TUBES] = { false };

#ifndef USE_HW_TUBE_SENS
uint8_t hw_tube_sens_read_all(void) {

    // Handle debug button press to insert new random tube
    if (button_is_pressed(DEBUG)) {

        // printf("Debug button pressed, inserting tube\n");
        // Find an unused tube
        int available_tubes[NUM_TUBES];
        int available_count = 0;
        
        for (int i = 0; i < NUM_TUBES; i++) {
            if (!used_tubes[i]) {
                available_tubes[available_count++] = i;
            }
        }
        
        // If tubes are available, pick a random one
        if (available_count > 0) {
            int random_idx = random() % available_count;
            int tube_index = available_tubes[random_idx];

            // printf("Inserting tube %d\n", tube_index);
            
            gSysControl.tube_present[tube_index] = true;
            used_tubes[tube_index] = true;
        }
    }

    return 0;
    
}
#endif