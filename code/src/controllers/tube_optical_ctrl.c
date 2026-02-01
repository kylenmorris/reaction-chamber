#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>

#include "tube_optical_ctrl.h"

// Read optical intensities into gOpticalInputs here
void tube_optical_ctrl_step(void) {
    for (int i = 0; i < NUM_TUBES - 6; i++) {                                                                
        gOpticalInputs.intensity[i] = (OPTICAL_REACTION_THRESHOLD + 100 - rand() % 100000);
    }
    for (int i = NUM_TUBES - 6; i < NUM_TUBES; i++) {
        gOpticalInputs.intensity[i] = rand() % 3000;
    }

}

