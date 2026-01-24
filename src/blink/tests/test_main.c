#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "data_structs.h"
#include "system_state_loop.h"
#include "heater_tests.h"

// #######################################################
// Heater tests
// #######################################################

int main(void) {
    printf("--- PathoScan Simulation Test Start ---\n");
    
    gSystemState = BOOT;
    run_system_state_loop_core0();

    heating_test();
    heating_test_low();
    temp_sensor_fail_test();

    printf("--- Tests Complete ---\n");

    return 0;
}