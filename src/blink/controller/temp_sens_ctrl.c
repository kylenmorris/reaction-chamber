#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>

#include "temp_sens_ctrl.h"

void temp_sens_init(void) {
    gTempStatus.chamber_temp = 0.0f;
}

void temp_sens_step(void) {
    gTempStatus.chamber_temp = readTemperatureSensor();
    gTempStatus.temp_low = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C);
    gTempStatus.temp_extreme = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C || 
                                        gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C);
    
}

float readTemperatureSensor(void) {
    if (gTempStatus.chamber_temp < TEMP_LOW_C - 0.5) {
        return gTempStatus.chamber_temp + 0.01f; // dummy increment for testing
    } else {
        return gTempStatus.chamber_temp;
    }
}