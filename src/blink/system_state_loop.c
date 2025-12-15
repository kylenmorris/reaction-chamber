#include "constants.h"

#include "data_structs.h"
#include "imodel_structs.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"


// Main system state loop and update
void run_system_state_loop() {
    
    temp_sens_ctrl_step();

    switch (gSystemState) {
        case IDLE:

            // no inputs means stay in idle
            if (!gButtonInput.wasPressed) {
                break;
            }

            gIdleMenuIM.needs_redraw = true;

            if (gButtonInput.lastPressed == UP) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 0;
            }
            
            if (gButtonInput.lastPressed == DOWN) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.selected_index = 1;
            }
            
            if (gButtonInput.lastPressed == SELECT) {
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

            if (gButtonInput.wasPressed && gButtonInput.lastPressed == BACK) {
                gButtonInput.wasPressed = false;  // Reset flag
                gIdleMenuIM.needs_redraw = true;
                gSystemState = IDLE;
            }

            if (gTempStatus.chamber_temp >= gTempStatus.target_temp) { // this should probably be a flag in gTempStatus
                gSystemState = REACTING;
            }

            break;

        case REACTING:
            gSystemState = RESULTS;

        case RESULTS:
            gSystemState = IDLE;

        case HISTORY:
            gSystemState = IDLE;
    }

}