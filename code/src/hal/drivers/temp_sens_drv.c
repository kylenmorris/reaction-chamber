#include "drivers.h"

#ifdef USE_HW_TEMP_SENS

#include <hardware/i2c.h>
#include "data_structs.h"

// adapted from 
// https://github.com/raspberrypi/pico-examples/blob/master/i2c/mcp9808_i2c/mcp9808_i2c.c
// into gemini
float mcp9808_convert_temp(uint8_t upper_byte, uint8_t lower_byte) {
    float temperature;

    // Mask the flags (Bits 15, 14, 13) out. 
    // 0x1F keeps only the Sign bit (bit 12) and the 4 integer bits.
    upper_byte = upper_byte & 0x1F; 

    // Check the SIGN bit (Bit 12 / 0x10)
    if ((upper_byte & 0x10) == 0x10) {
        // Negative Temperature logic
        upper_byte = upper_byte & 0x0F; // Clear sign bit for math
        temperature = 256.0 - (((float)upper_byte * 16.0) + ((float)lower_byte / 16.0));
    } else {
        // Positive Temperature logic
        // Mask upper_byte to 0x0F to ensure no flags interfere with math
        temperature = (((float)upper_byte) * 16.0) + ((float)lower_byte / 16.0);
    }

    return temperature;
}

float hw_read_temperature_sensor(int address) {

    // printf("Reading temperature sensor at address 0x%02X\n", address);
    const uint8_t reg = 0x05;
    uint8_t buf[2];
    
    int w = i2c_write_timeout_us(i2c0, address, &reg, 1, true, 50000);
    int r = i2c_read_timeout_us(i2c0, address, buf, 2, false, 50000);

    if ((r < 0) || (w < 0)) {
        gSystemInfo.temp_sensor_success_count--;
        if (gSystemInfo.temp_sensor_success_count < -5) {
            gSystemError.current_error = ERROR_TEMP_SENSOR_FAULT;
            gSystemInfo.temp_sensor_success_count = -6;
        }
        return -999;
    } else {
        gSystemInfo.temp_sensor_success_count++;
    }

    float temp = mcp9808_convert_temp(buf[0], buf[1]);

    // uint16_t raw = ((uint16_t)buf[0] << 8) | buf[1];
    // raw &= 0x1FFF;

    // float temp = raw * 0.0625f;

    // if (raw & 0x1000)
    //     temp -= 256.0f;

    if (temp < -40.0f || temp > 125.0f) {
        // Out of range, likely a faulty reading
        gSystemInfo.temp_sensor_success_count--;
        if (gSystemInfo.temp_sensor_success_count < -5) {
            gSystemError.current_error = ERROR_TEMP_SENSOR_FAULT;
            gSystemInfo.temp_sensor_success_count = -6;
        }
        return -999;
    }

    return temp;
}

#endif


#ifndef USE_HW_TEMP_SENS

#include "data_structs.h"

float hw_read_temperature_sensor(int sensor_id) {

    if (gSimParams.force_temp_sensor_fault) return -999.0f;

    // if heater is on, temp goes up
    if (gHeaterState.heaterOn == true) {
        gTempStatus.chamber_temp += gSimParams.heat_rate;
    } else {
        if (gTempStatus.chamber_temp > gSimParams.ambient_temp) {
            gTempStatus.chamber_temp -= gSimParams.cool_rate;
        }
    }

    return gTempStatus.chamber_temp;
}
#endif
