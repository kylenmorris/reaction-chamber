#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../include/model/data_structs.h"
#include "../include/core/system_state_loop.h"
#include "include/heater_tests.h"

void tick_system(int cycles) {
    for (int i = 0; i < cycles; i++) {
        run_system_state_loop_core0();
    }
}

int main(void) {
    printf("--- PathoScan Simulation Test Start ---\n");
    
    gSystemState = BOOT;
    run_system_state_loop_core0();

    json_test();
    heating_test_high();
    heating_test_low();
    temp_sensor_failure_test();

    printf("--- Tests Complete ---\n");

    return 0;
}