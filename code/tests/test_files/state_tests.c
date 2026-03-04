#include "../include/state_tests.h"
#include "../include/definitions.h"

#include <stdio.h>

// Clangd can't find these because compiling for windows doesn't result in a 
// compile_commands.json file. I don't know how to fix that at the moment.
#include "../../include/model/data_structs.h"
#include "../../include/core/system_state_loop.h"
#include "../../include/model/data_helpers.h"


void test_state_to_reacting_on_temp(void) {
    printf("Running Test: test_test_state_to_reacting_on_temp...");

    gTempStatus.chamber_temp = 40; 
    gSimParams.heat_rate = 1.0f;

    gSystemState = HEATING;

    for (int i = 0; i < 100; i++) {
        run_system_state_loop_core0();
    }

    EXPECT_ONCE(gSystemState == REACTING, "System did not move to reacting.");        

    reset_sim_params();
    printf("    FINISHED\n");
}

void state_to_results_on_finish(void) {

}

