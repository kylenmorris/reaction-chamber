#include "temp_sens_ctrl.h"

#include "drivers.h"

#include "constants.h"
#include "data_structs.h"

void temp_sens_ctrl_step(void) {
    gTempStatus.chamber_temp = hw_read_temperature_sensor(1);
    
    gTempStatus.temp_low = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C);
    gTempStatus.temp_extreme = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C || 
                                        gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C);
    gTempStatus.target_reached = (gTempStatus.chamber_temp >= gTempStatus.target_temp);
}
