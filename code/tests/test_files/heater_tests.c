#include "../include/heater_tests.h"
#include "../include/definitions.h"

#include <stdio.h>

// Clangd can't find these because compiling for windows doesn't result in a 
// compile_commands.json file. I don't know how to fix that at the moment.
#include "../../include/model/data_structs.h"
#include "../../include/core/system_state_loop.h"
#include "../../include/model/data_helpers.h"

// Check behaviour during temperature sensor failure in all states

void temp_sensor_failure_test() {
    printf("Running Test: Temperature Sensor Failure...\n");

    gSimParams.force_temp_sensor_fault = true;

    SystemState test_cases[] = {IDLE, BOOT, RESULTS, HISTORY, HEATING, REACTING};
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_cases; i++) {
        SystemState currentState = test_cases[i];
        gSystemState = currentState;
        
        const char* stateName = get_system_state_string(currentState); 
        // printf("  Checking state: %s\n", stateName);

        for (int cycle = 0; cycle < TEST_CYCLES; cycle++) {
            run_system_state_loop_core0();

            EXPECT(gHeaterState.heaterOn == false, 
                   "Heater active during sensor fault in state %s", 
                   stateName);        
        }
    }

    gSimParams.force_temp_sensor_fault = false;

    EXPECT_ONCE(gTempStatus.chamber_temp < 0, "Error: Simulation failed to inject sensor fault");

    reset_sim_params();
    printf("Test Temperature Sensor Failure: FINISHED\n");
}

// Check the heater turns on when it should

void heating_test_low() {
    printf("Running Test: Cold Safety Check...\n");

    gTempStatus.chamber_temp = 0; 

    gSystemState = HEATING;

    for (int i = 0; i < TEST_CYCLES; i++) {

        gTempStatus.chamber_temp += 1.0f;

        run_system_state_loop_core0();

        if (gTempStatus.chamber_temp < TEMP_LOW_LOW_C) {
            EXPECT(gHeaterState.heaterOn == true, "Heater should be enabled below %dC", TEMP_LOW_LOW_C);        
        }
    }

    // EXPECT_ONCE(gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C, "Test did not move temp above %dC", TEMP_HIGH_HIGH_C);        

    reset_sim_params();
    printf("Test Cold: FINISHED\n");
}

// Check heater turns off when it should

void heating_test_high() {
    printf("Running Test: Overheat Safety Check...\n");

    gSimParams.ambient_temp = 25.0f;
    gSimParams.heat_rate = 1.0f; // Increase 1 degree per tick
    gTempStatus.chamber_temp = 60.0f; // Start near limit
    gSimParams.cool_rate = 0.1f;

    gSystemState = HEATING;


    SystemState test_cases[] = {IDLE, BOOT, RESULTS, HISTORY, HEATING, REACTING};
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_cases; i++) {
        SystemState currentState = test_cases[i];
        gSystemState = currentState;
        
        const char* stateName = get_system_state_string(currentState); 
        // printf("  Checking state: %s\n", stateName);

        for (int cycle = 0; cycle < TEST_CYCLES; cycle++) {
            
            gTempStatus.chamber_temp += 1.0f;
            run_system_state_loop_core0();

            if (gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C) {
                EXPECT(gHeaterState.heaterOn == false, "Heater should be safety-disabled above %dC in state %s", TEMP_HIGH_HIGH_C, stateName);        
            }  
        }
    }

    EXPECT_ONCE(gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C, "Test did not move temp above %dC", TEMP_HIGH_HIGH_C);        

    reset_sim_params();
    printf("Test Overheat: FINISHED\n");
}
