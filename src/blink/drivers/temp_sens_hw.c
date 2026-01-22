#include "constants.h"
#include "data_structs.h"

#include "temp_sens_interface.h"

static float readTemperatureSensor(void) {
    if (gTempStatus.chamber_temp < TEMP_LOW_C + 1.5) {
        return gTempStatus.chamber_temp + 0.1f;
    } else {
        return gTempStatus.chamber_temp;
    }
}

void temp_sens_ctrl_init(void) {
    gTempStatus.chamber_temp = 0.0f;
}
