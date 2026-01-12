#include "constants.h"

#include "data_structs.h"
#include "imodel_structs.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"
#include "test_manager.h"


// Main system state loop and update
void run_system_state_loop() {
    
    temp_sens_ctrl_step();

    switch (gSystemState) {
        case IDLE:

            // no inputs means stay in idle, ,keeping this for redraw limiting for now
            if (!gButtonInput.wasPressed) {
                break;
            }

            gIdleMenuIM.needs_redraw = true;

            if (button_ctrl_is_pressed(UP)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 0;
            }
            
            if (button_ctrl_is_pressed(DOWN)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 1;
            }
            
            if (button_ctrl_is_pressed(SELECT)) {
                gButtonInput.wasPressed = false;  // Reset flag

                if (gIdleMenuIM.selected_index == 0) {
                    heater_ctrl_init();
                    temp_sens_ctrl_init();
                    
                    gHeatingMenuIM.needs_redraw = true;
                    gSystemState = HEATING;
                }
                else if (gIdleMenuIM.selected_index == 1) {
                    gSystemState = HISTORY;
                }
            }

            // back does nothing in idle

            break;

        case HEATING:

            if (button_ctrl_is_pressed(BACK)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            if (gTempStatus.chamber_temp >= gTempStatus.target_temp) { // this should probably be a flag in gTempStatus
                test_manager_start();
                gTestRunningIM.needs_redraw = true;
                gSystemState = REACTING;
            }

            break;

        case REACTING:

            // Back to idle
            if (button_ctrl_is_pressed(BACK)) {
                gButtonInput.wasPressed = false;  // Reset flag
                // heater_ctrl_shutdown();
                // temp_sens_ctrl_shutdown();
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            // Reaction complete
            if (gTestStatus.completed) {
                // heater_ctrl_shutdown();
                // temp_sens_ctrl_shutdown();
                gResultsIM.needs_redraw = true;
                gSystemState = RESULTS;
            }

            test_manager_step();

            break;

        case RESULTS:
            
            // Select or Back to go to idle
            if (button_ctrl_is_pressed(BACK) || button_ctrl_is_pressed(SELECT)) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            break;

        case HISTORY:
            gSystemState = IDLE;
    }

}