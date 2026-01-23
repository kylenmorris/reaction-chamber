#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "constants.h"
#include "data_structs.h"
#include "system_state_loop.h"
#include "heater_interface.h"


void test_overheat(void) {
    printf("Running Test: Overheat Safety Check...\n");

    // 1. Setup Simulation "God Mode"
    gSimParams.ambient_temp = 25.0f;
    gSimParams.heat_rate = 1.0f; // Increase 1 degree per tick
    gTempStatus.chamber_temp = 60.0f; // Start near limit
    
    // 2. Force system into HEATING state
    gSystemState = HEATING;

    // 3. Run the loop until temp is way too high
    for (int i = 0; i < 20; i++) {
        run_system_state_loop_core0();

        // Check: If temp is > 69, heater MUST be toggled off by heater_ctrl
        if (gTempStatus.chamber_temp > 69.0f) {
            // This assumes your hw_heater_toggle records state in a global or gSimParams
            // Or you check your internal gHeaterStatus struct
            assert(gHeaterStatus.is_on == false && "Heater must be OFF above 69C!");
        }
    }

    printf("Test Overheat: PASSED\n");
}