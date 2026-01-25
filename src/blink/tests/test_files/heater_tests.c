#include "../include/heater_tests.h"
#include "../include/definitions.h"

#include <stdio.h>
#include <stdlib.h>

// Clangd can't find these because compiling for windows doesn't result in a 
// compile_commands.json file. I don't know how to fix that at the moment.
#include "../../include/model/data_structs.h"
#include "../../include/core/system_state_loop.h"
#include "../../include/storage/json_helper.h"
#include "../../include/storage/sd_drv.h"

void json_test() {
    char *string = serialize_test_results();
    save_to_file(string);

    free(string);
}

void temp_sensor_fail_test() {
    printf("Running Test: Temperature Sensor Failure...\n");

    gSimParams.force_temp_sensor_fault = true;

    gSystemState = HEATING;
    
    for (int i = 0; i < TEST_CYCLES; i++) {

        run_system_state_loop_core0();

        EXPECT(gHeaterState.heaterOn == false, "Heater should be safety-disabled when temp sensors fail");        
    }

    EXPECT_ONCE(gTempStatus.chamber_temp < 0, "Sensors did not fail");

    printf("Test Overheat: FINISHED\n");
}

void heating_test_low() {
    printf("Running Test: Overheat Safety Check...\n");

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

    printf("Test Overheat: FINISHED\n");
}

void heating_test() {
    printf("Running Test: Overheat Safety Check...\n");

    gSimParams.ambient_temp = 25.0f;
    gSimParams.heat_rate = 1.0f; // Increase 1 degree per tick
    gTempStatus.chamber_temp = 60.0f; // Start near limit
    gSimParams.cool_rate = 0.1f;

    gSystemState = HEATING;

    for (int i = 0; i < TEST_CYCLES; i++) {

        gTempStatus.chamber_temp += 1.0f;

        run_system_state_loop_core0();

        if (gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C) {
            EXPECT(gHeaterState.heaterOn == false, "Heater should be safety-disabled above %dC", TEMP_HIGH_HIGH_C);        
        }
    }

    EXPECT_ONCE(gTempStatus.chamber_temp > TEMP_HIGH_HIGH_C, "Test did not move temp above %dC", TEMP_HIGH_HIGH_C);        

    printf("Test Overheat: FINISHED\n");
}
