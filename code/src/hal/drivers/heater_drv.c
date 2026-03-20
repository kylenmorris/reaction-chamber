#include "drivers.h"


#ifdef USE_HW_HEATER

#include <hardware/gpio.h>

void hw_heater_toggle(bool enable) {
    if (enable) {
        gpio_put(HEATER_PIN, 1);
        // printf("Heater pin ON\n");
    } else {
        gpio_put(HEATER_PIN, 0);
        // printf("Heater pin OFF\n");
    }
}

#endif


#ifndef USE_HW_HEATER

#include "data_structs.h"

void hw_heater_toggle(bool enable) {
    gHeaterState.heaterOn = enable;
}

#endif
