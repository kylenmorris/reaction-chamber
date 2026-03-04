#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "../include/model/data_structs.h"
#include "../include/model/data_helpers.h"
#include "../include/core/system_state_loop.h"
#include "include/heater_tests.h"
#include "include/state_tests.h"

void tick_system(int cycles) {
    for (int i = 0; i < cycles; i++) {
        run_system_state_loop_core0();
    }
}

int main(void) {
    printf("--- PathoScan Simulation Test Start ---\n");
    
    reset_sim_params();
    
    gSystemState = BOOT;
    run_system_state_loop_core0();

    test_heater_shutoff_on_overheat();
    test_heater_turns_on_when_heating();
    test_heater_not_active_during_temp_sensor_failure();

    test_state_to_reacting_on_temp();

    printf("--- Tests Complete ---\n");

    return 0;
}