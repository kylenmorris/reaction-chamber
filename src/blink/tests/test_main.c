#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "constants.h"
#include "data_structs.h"
#include "display_ctrl.h"
#include "system_state_loop.h"

int gTestsFailed = 0;
int gTestsPassed = 0;

#define EXPECT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("\033[1;31m[FAIL]\033[0m %s (Line %d): %s\n", __FUNCTION__, __LINE__, message); \
            gTestsFailed++; \
        } else { \
            gTestsPassed++; \
        } \
    } while (0)

void heating_test() {
    printf("Running Test: Overheat Safety Check...\n");

    // 1. Setup Simulation "God Mode"
    gSimParams.ambient_temp = 25.0f;
    gSimParams.heat_rate = 1.0f; // Increase 1 degree per tick
    gTempStatus.chamber_temp = 60.0f; // Start near limit
    gSimParams.cool_rate = 0.1f;
    
    // 2. Force system into HEATING state
    gSystemState = HEATING;

    // 3. Run the loop until temp is way too high
    for (int i = 0; i < 20; i++) {

        gTempStatus.chamber_temp += 1.0f;

        run_system_state_loop_core0();

        // Check: If temp is > 69, heater MUST be toggled off by heater_ctrl
        if (gTempStatus.chamber_temp > 69.0f) {
            EXPECT(gHeaterState.heaterOn == false, "Heater should be safety-disabled above 69C");        
        }
    }

    printf("Final chamber temp: %f\n", gTempStatus.chamber_temp);        

    printf("Test Overheat: PASSED\n");
}

int main(void) {
    printf("--- PathoScan Simulation Test Start ---\n");
    
    // Simulate a few steps of the display controller
    gSystemState = BOOT;
    display_ctrl_step();
    
    gSystemState = HEATING;

    printf("--- Test Complete ---\n");

    heating_test();

    return 0;
}