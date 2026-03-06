#include "temp_sens_ctrl.h"

#include "data_helpers.h"
#include "drivers.h"

#include "constants.h"
#include "data_structs.h"
#include <stdio.h>


float inject_debug_value(float temp) {

    return temp;
}

// Read temp status into gTempStatus
void temp_sens_ctrl_step(void) {

    gTempStatus.temp_readings[0] = hw_read_temperature_sensor(TEMP_SENSOR_0_I2C_ADDRESS);
    gTempStatus.temp_readings[1] = hw_read_temperature_sensor(TEMP_SENSOR_1_I2C_ADDRESS);
    // gTempStatus.temp_readings[2] = hw_read_temperature_sensor(TEMP_SENSOR_2_I2C_ADDRESS);
    // gTempStatus.temp_readings[3] = hw_read_temperature_sensor(TEMP_SENSOR_3_I2C_ADDRESS);

    gTempStatus.chamber_temp = gTempStatus.temp_readings[0]; // Assuming chamber temperature is from the second sensor

    gTempStatus.temp_low = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C);
    gTempStatus.temp_extreme = (gTempStatus.chamber_temp < TEMP_LOW_LOW_C || 
                                        gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C);
    gTempStatus.target_reached = (gTempStatus.chamber_temp >= gTempStatus.target_temp);
}
