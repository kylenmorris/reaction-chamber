#include "constants.h"
#include "data_structs.h"
#include <stdio.h>
#include <stdlib.h>

#include "tube_sens_ctrl.h"
#include "button_ctrl.h"

// Track which tubes have been inserted via debug button
static bool used_tubes[NUM_TUBES] = { false };

void tube_sens_ctrl_init(void) {
    // Initialize tube sensors here
    for (int i = 0; i < NUM_TUBES; i++) {
        used_tubes[i] = false;    
        gTestStatus.tubes[i].state = EMPTY;
    }


    // Reset tube statuses - potentially
    // for (int i = 0; i < NUM_TUBES; i++) {
    //     gTestStatus.tubes[i].state = EMPTY;
    //     gTestStatus.tubes[i].result = UNKNOWN;
    //     gTestStatus.tubes[i].start_time = 0;
    //     gTestStatus.tubes[i].last_update = 0;
    //     gTestStatus.tubes[i].is_positive_control = false;
    //     gTestStatus.tubes[i].is_negative_control = false;
    //     gTestStatus.tubes[i].positive_detected = false;
    //     gTestStatus.tubes[i].ct_time = 0;
    // }

}

// Detects the insertion of tubes and might do other stuff later
void tube_sens_ctrl_step(void) {

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
            int random_idx = rand() % available_count;
            int tube_index = available_tubes[random_idx];

            // printf("Inserting tube %d\n", tube_index);
            
            gSysControl.tube_present[tube_index] = true;
            used_tubes[tube_index] = true;
        }
    }

}