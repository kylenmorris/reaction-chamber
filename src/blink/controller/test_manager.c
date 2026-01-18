#include "test_manager.h"

#include "data_structs.h"
#include "pico/time.h"
#include "constants.h"

// Timers for temperature violations
static uint32_t temp_low_start = 0;
static uint32_t temp_extreme_start = 0;

void test_manager_init(void) {
    // Initialize test manager here
}


void test_manager_start(void) {
    gTestStatus.reaction_active = true;

    gTestStatus.reaction_start_time = to_ms_since_boot(get_absolute_time());
    gTestStatus.test_invalid = false;
    gTestStatus.completed = false;

    temp_low_start = 0;
    temp_extreme_start = 0;
}

void test_manager_determine_results(void) {
    
    for (int i = 0; i < NUM_TUBES; i++) {

        // positive and negative control checks
        if (gTestStatus.tubes[i].is_positive_control 
            && !gTestStatus.tubes[i].positive_detected) {
            gTestStatus.test_invalid = true;
        }

        if (gTestStatus.tubes[i].is_negative_control 
            && gTestStatus.tubes[i].positive_detected) {
            gTestStatus.test_invalid = true;
        }

    }

    for (int i = 0; i < NUM_TUBES; i++) {

        if (gTestStatus.test_invalid) {
            gTestStatus.tubes[i].result = INVALID_RESULT;
        } else if (gTestStatus.tubes[i].positive_detected) {
            gTestStatus.tubes[i].result = POSITIVE;
        } else {
            gTestStatus.tubes[i].result = NEGATIVE;
        }
    }

}

void test_manager_stop(void) {
    test_manager_determine_results();
}


void test_manager_step(void) {

    uint32_t now_ms = to_ms_since_boot(get_absolute_time());
    
    gTestStatus.reaction_total_time = now_ms - gTestStatus.reaction_start_time;
    
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

        uint32_t tube_reaction_time = now_ms - gTestStatus.tubes[i].start_time;

        // Check for new tube insertions
        if (gSysControl.tube_present[i] && gTestStatus.tubes[i].state == EMPTY) {
            gTestStatus.tubes[i].state = RUNNING;
            gTestStatus.tubes[i].start_time = now_ms;
            // gSysControl.tube_present[i] = false; // Reset flag
        }

        // Check for tube positive result
        if (gTestStatus.tubes[i].state == RUNNING && 
            gOpticalInputs.intensity[i] >= OPTICAL_REACTION_THRESHOLD &&
            !gTestStatus.tubes[i].positive_detected) {
            
            gTestStatus.tubes[i].positive_detected = true;
            gTestStatus.tubes[i].ct_time = tube_reaction_time;
            gTestStatus.tubes[i].state = COMPLETED;
        }

        // Check for tube completions
        if (gTestStatus.tubes[i].state == RUNNING &&
            tube_reaction_time >= REACTION_DURATION_MS) {

            gTestStatus.tubes[i].state = COMPLETED;

        }

        
    }

    // check for test completion
    if (gTestStatus.reaction_total_time >= REACTION_DURATION_MS) {
        gTestStatus.reaction_active = false;
        gTestStatus.completed = true;
    }

}
