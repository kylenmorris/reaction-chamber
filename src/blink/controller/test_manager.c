#include "test_manager.h"

#include "data_structs.h"
#include "pico/time.h"
#include "constants.h"

static uint32_t temp_low_start = 0;
static uint32_t temp_extreme_start = 0;

void test_manager_init(void) {
    // Initialize test manager here
}

void test_manager_start(void) {
    // Start a new test here
}

void test_manager_step(void) {

    uint32_t now_ms = to_ms_since_boot(get_absolute_time());
    
    if (!gTestStatus.reaction_active) {
        return; // No active test
    }

    // Check for low temp
    if (gTempStatus.temp_low) {
        if (temp_low_start == 0) {
            temp_low_start = now_ms;
        }
        if ((now_ms - temp_low_start) >= TEMP_LOW_DURATION_MS) {
            gTestStatus.test_invalid = true;
        }
    } else {
        temp_low_start = 0;
    }

    // Check for extreme temp
    if (gTempStatus.temp_extreme) {
        if (temp_extreme_start == 0) {
            temp_extreme_start = now_ms;
        }
        if ((now_ms - temp_extreme_start) >= TEMP_EXTREME_DURATION_MS) {
            gTestStatus.test_invalid = true;
        }
    } else {
        temp_extreme_start = 0;
    }

    for (int i = 0; i < NUM_TUBES; i++) {

        // Check for new reactions
        if (gSysControl.start_tube[i]) {
            gTestStatus.tubes[i].state = RUNNING;
            gTestStatus.tubes[i].start_time = now_ms;
            gSysControl.start_tube[i] = false; // Reset flag
        }

        
    }

}
