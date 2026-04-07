#include "tube_optical_ctrl.h"

#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>
#include "constants.h"
#include "drivers.h"

void set_all_inserted_tubes_detected() {
    for (int i = 0; i < NUM_TUBES; i++) {
        if (gSysControl.tube_present[i]) {
            gOpticalInputs.intensity[i] = OPTICAL_REACTION_THRESHOLD + 100; // Set above threshold
        } else {
            gOpticalInputs.intensity[i] = 0; // No tube, no intensity
        }
    }
}

// Read optical intensities into gOpticalInputs here
void tube_optical_ctrl_step(void) {
    for (int i = 0; i < NUM_TUBES / 2; i++) {
        int rev = (NUM_TUBES / 2) - 1 - i;   // reverse index for group of 5

        gOpticalInputs.intensity[i] =
            hw_adc_read_raw(SPI1_CSn_ADC0_PIN, rev + 3);

        gOpticalInputs.intensity[i + NUM_TUBES / 2] =
            hw_adc_read_raw(SPI1_CSn_ADC1_PIN, i + 0);
    }
}
