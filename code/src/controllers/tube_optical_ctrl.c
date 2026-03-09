#include "tube_optical_ctrl.h"

#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>
#include "constants.h"
#include "drivers.h"

// Read optical intensities into gOpticalInputs here
void tube_optical_ctrl_step(void) {
    for (int i = 0; i < NUM_TUBES / 2; i++) {
        int rev = i;   // reverse index for group of 5

        gOpticalInputs.intensity[i] =
            hw_adc_read_raw(SPI1_CSn_ADC0_PIN, rev + 3);

        gOpticalInputs.intensity[i + NUM_TUBES / 2] =
            hw_adc_read_raw(SPI1_CSn_ADC1_PIN, rev + 1);
    }
}
