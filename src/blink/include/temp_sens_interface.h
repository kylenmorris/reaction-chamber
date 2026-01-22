#ifndef TEMP_SENS_INTERFACE_H
#define TEMP_SENS_INTERFACE_H

// Initialize I2C for temperature sensors
void temp_sens_ctrl_init(void);

// Read data from one sensor
float hw_read_temperature_sensor(int sensor_id);

#endif