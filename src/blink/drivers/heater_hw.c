#include "heater_interface.h"

#include <stdio.h>

void heater_init(void) {
    printf("Heater control initialized (simulated).\n");
}

void heater_toggle(bool enable) {
    if (enable) {
        printf("Heater enabled (simulated).\n");
    } else {
        printf("Heater disabled (simulated).\n");
    }
}