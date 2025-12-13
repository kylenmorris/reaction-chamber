#include "constants.h"
#include "data_structs.h"

#include "temp_sens_ctrl.h"

void temp_sens_init(void) {

}

void temp_sens_step(void) {
    gTemperatureStatus.chamber_temp = readTemperatureSensor();
}

float readTemperatureSensor(void) {
    return 25.0;
}