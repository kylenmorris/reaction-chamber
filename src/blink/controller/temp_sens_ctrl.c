#include "constants.h"
#include "data_structs.h"
#include <stdlib.h>

#include "temp_sens_ctrl.h"

void temp_sens_init(void) {

}

void temp_sens_step(void) {
    gTempStatus.chamber_temp = readTemperatureSensor();
    gTempStatus.temp_low = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C);
    gTempStatus.temp_extreme = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C || 
                                        gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C);
}

float readTemperatureSensor(void) {
    return rand() % 3 + 61.1f; // between 61.1 and 63.9
}