#include "../include/state_tests.h"
#include "../include/definitions.h"

#include <stdio.h>
#include <stdlib.h>

// Clangd can't find these because compiling for windows doesn't result in a 
// compile_commands.json file. I don't know how to fix that at the moment.
#include "../../include/model/data_structs.h"
#include "../../include/core/system_state_loop.h"


void state_all_to_idle_on_back(void) {

}

void state_to_heating_on_select(void) {

}

void state_to_reacting_on_temp(void) {
    printf("Running Test: Heating -> Reacting...\n");

    gTempStatus.chamber_temp = 40; 
    gSimParams.heat_rate = 1.0f;

    gSystemState = HEATING;

    for (int i = 0; i < 100; i++) {
        run_system_state_loop_core0();
    }

    EXPECT_ONCE(gSystemState == REACTING, "System did not move to reacting.");        

    reset_sim_params();
    printf("Test State: FINISHED\n");
}

void state_to_results_on_finish(void) {

}

