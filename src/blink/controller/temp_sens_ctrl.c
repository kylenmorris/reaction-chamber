#include "constants.h"
#include "data_structs.h"

#include "temp_sens_ctrl.h"
#include "button_ctrl.h"

// Simulated *** FOR NOW ***
static float readTemperatureSensor(void) {
    // increase faster if debug is pressed, limiting more to avoid overshoots
    if (gTempStatus.chamber_temp < TEMP_LOW_C + 2.8 
            && button_ctrl_is_pressed(DEBUG)) {
        return gTempStatus.chamber_temp + 0.5f;
    }
    else if (gTempStatus.chamber_temp < TEMP_LOW_C + 1.5) {
        return gTempStatus.chamber_temp + 0.1f;
    } else {
        return gTempStatus.chamber_temp;
    }
}

void temp_sens_ctrl_init(void) {
    gTempStatus.chamber_temp = 0.0f;
}

void temp_sens_ctrl_step(void) {
    gTempStatus.chamber_temp = readTemperatureSensor();
    gTempStatus.temp_low = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C);
    gTempStatus.temp_extreme = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C || 
                                        gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C);
    gTempStatus.target_reached = (gTempStatus.chamber_temp >= gTempStatus.target_temp);
}
