#include "temp_sens_ctrl.h"

#include "drivers.h"

#include "constants.h"
#include "data_structs.h"
#include <stdio.h>

// Read temp status into gTempStatus
void temp_sens_ctrl_step(void) {
    gTempStatus.chamber_temp = hw_read_temperature_sensor(TEMP_SENSOR_0_I2C_ADDRESS);
    // printf("Chamber temp 1: %.2f°C\n", gTempStatus.chamber_temp);
    
    // gTempStatus.chamber_temp = hw_read_temperature_sensor(TEMP_SENSOR_2_I2C_ADDRESS);
    // printf("Chamber temp 2: %.2f°C\n", gTempStatus.chamber_temp);

    // gTempStatus.chamber_temp = hw_read_temperature_sensor(TEMP_SENSOR_3_I2C_ADDRESS);
    // printf("Chamber temp 3: %.2f°C\n", gTempStatus.chamber_temp);

    // gTempStatus.chamber_temp = hw_read_temperature_sensor(TEMP_SENSOR_1_I2C_ADDRESS);
    // printf("Chamber temp 4: %.2f°C\n", gTempStatus.chamber_temp);

    gTempStatus.temp_low = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C);
    gTempStatus.temp_extreme = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C || 
                                        gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C);
    gTempStatus.target_reached = (gTempStatus.chamber_temp >= gTempStatus.target_temp);
}
