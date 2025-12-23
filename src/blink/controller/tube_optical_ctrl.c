#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>

#include "tube_optical_ctrl.h"


void tube_optical_ctrl_init(void) {
    // Initialize optical reading hardware here
}

void tube_optical_ctrl_step(void) {
    // Read optical intensities into gOpticalInputs here
    for (int i = 0; i < NUM_TUBES; i++) {
        gOpticalInputs.intensity[i] = gOpticalInputs.intensity[i] + (rand() % 100); // Replace with actual reading
    }
}

