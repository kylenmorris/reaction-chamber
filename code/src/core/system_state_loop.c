#include "constants.h"
#include "data_structs.h"
#include "imodel_structs.h"

#include "test_manager.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"
#include "tube_sens_ctrl.h"
#include "sd_ctrl.h"
#include <stdbool.h>

void run_system_state_loop_core1() {
    button_ctrl_step(); 
    display_ctrl_step();
}

// Main system state loop and update
void run_system_state_loop_core0() {
    
    temp_sens_ctrl_step();
    heater_ctrl_step();

    switch (gSystemState) {
        case BOOT:
            gIdleMenuIM.needs_redraw = true;
            gSystemState = IDLE;
            break;

        case IDLE:

            // no inputs means stay in idle, keeping this for redraw limiting for now
            if (!gButtonInput.wasPressed) {
                break;
            }

            gIdleMenuIM.needs_redraw = true;

            if (button_is_pressed(UP)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 0;   // Update selection
            }
            
            if (button_is_pressed(DOWN)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 1;   // Update selection
            }
            
            if (button_is_pressed(SELECT)) {
                gButtonInput.wasPressed = false;  // Reset flag

                if (gIdleMenuIM.selected_index == 0) { // Go to heating                    
                    gHeatingMenuIM.needs_redraw = true;
                    gSystemState = HEATING;
                }
                else if (gIdleMenuIM.selected_index == 1) { // go to history
                    gHistoryIM.needs_redraw = true;
                    gSystemState = HISTORY;
                }
            }

            // back does nothing in idle

            break;

        case HEATING:

            if (button_is_pressed(BACK)) {  // back to idle
                gButtonInput.wasPressed = false;        // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            if (gTempStatus.target_reached) {           // target temp reached, start test
                test_manager_start();
                gTestRunningIM.needs_redraw = true;
                gSystemState = REACTING;
            }

            break;

        case REACTING:

            // Back to idle
            if (button_is_pressed(BACK)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            // Reaction complete
            if (gTestStatus.completed) {
                test_manager_stop();
                gResultsIM.needs_redraw = true;
                gSystemState = RESULTS;
            }

            tube_sens_ctrl_step();
            test_manager_step();
            tube_optical_ctrl_step();
            
            break;

        case RESULTS:
            
            // Select or Back to go to idle
            if (button_is_pressed(BACK) || button_is_pressed(SELECT)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            break;

        case HISTORY:
            save_test_results_to_file("test.json");
            load_test_from_filename("test.json");
            gSystemState = IDLE;
    }

}