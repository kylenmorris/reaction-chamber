#include "constants.h"
#include "data_structs.h"

#include "temp_sens_ctrl.h"

static float readTemperatureSensor(void) {
    if (gTempStatus.chamber_temp < TEMP_LOW_C - 0.5) {
        return gTempStatus.chamber_temp + 0.01f;
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
    
}
