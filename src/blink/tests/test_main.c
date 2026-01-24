#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "constants.h"
#include "data_structs.h"
#include "display_ctrl.h"
#include "system_state_loop.h"

int gTestsFailed = 0;
int gTestsPassed = 0;

#define EXPECT_ONCE(condition, format, ...) \
    if (!(condition)) { \
        printf("\033[1;31m[FAIL]\033[0m %s (Line %d): ", __FUNCTION__, __LINE__); \
        printf(format, ##__VA_ARGS__); \
        printf("\n"); \
        gTestsFailed++; \
    } else { \
        gTestsPassed++; \
    } \

#define EXPECT(condition, format, ...) \
    if (!(condition)) { \
        printf("\033[1;31m[FAIL]\033[0m %s (Line %d): ", __FUNCTION__, __LINE__); \
        printf(format, ##__VA_ARGS__); \
        printf("\n"); \
        gTestsFailed++; \
        break; \
    } else { \
        gTestsPassed++; \
    } \


// #######################################################
// Heater tests
// #######################################################

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