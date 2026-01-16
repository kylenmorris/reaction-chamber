#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>

#include "tube_optical_ctrl.h"


void tube_optical_ctrl_init(void) {
    // Initialize optical reading hardware here
}

// Simulated *** FOR NOW ***
void tube_optical_ctrl_step(void) {
    // Read optical intensities into gOpticalInputs here
    for (int i = 0; i < NUM_TUBES; i++) {
                                                                              
        gOpticalInputs.intensity[i] = (OPTICAL_REACTION_THRESHOLD + 100 - rand() % 50000);
    }
}

