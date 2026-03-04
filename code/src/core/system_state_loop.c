#include "system_state_loop.h"
#include "constants.h"
#include "data_structs.h"
#include "data_helpers.h"

#include "test_manager.h"

#include "button_ctrl.h"
#include "temp_sens_ctrl.h"
#include "heater_ctrl.h"
#include "tube_optical_ctrl.h"
#include "display_ctrl.h"
#include "tube_sens_ctrl.h"
#include "sd_ctrl.h"
#include <stdbool.h>
#include <stdio.h>

// ####################################
// HELPERS
// ####################################

static void move_to_idle() {
    gIdleMenuIM.needs_redraw = true;
    gSystemState = IDLE;
}

static void move_to_heating() {
    gHeatingMenuIM.needs_redraw = true;
    gSystemState = HEATING;
}

static void move_to_reacting() {
    gTestRunningIM.needs_redraw = true;
    gSystemState = REACTING;
}

static void move_to_results() {
    gResultsIM.needs_redraw = true;
    gSystemState = RESULTS;
}

static void move_to_history() {
    gHistoryIM.needs_redraw = true;
    gSystemState = HISTORY;
}

// ####################################
// MAIN LOOPS
// ####################################

void run_system_state_loop_core1() {
    display_ctrl_step();
}

void run_system_state_loop_core0() {
    
    temp_sens_ctrl_step();
    heater_ctrl_step();
    button_ctrl_step(); 
    tube_sens_ctrl_step();

    if (gSystemError.current_error != ERROR_NONE) {
        if (handle_button_press(SELECT)) {
            gSystemError.current_error = ERROR_NONE;
            gSystemError.needs_display = false;
            return;
        }
    }   

    switch (gSystemState) {
        case BOOT:
            move_to_idle();
            break;

        case IDLE:

            if (!gButtonInput.wasPressed) {
                break;
            }

            gIdleMenuIM.needs_redraw = true;

            if (handle_button_press(UP)) {
                gIdleMenuIM.selected_index = 0;   // Update selection
            }
            
            if (handle_button_press(DOWN)) {
                gIdleMenuIM.selected_index = 1;   // Update selection
            }
            
            if (handle_button_press(SELECT)) {

                if (gIdleMenuIM.selected_index == 0) {             
                    move_to_heating();
                }
                else if (gIdleMenuIM.selected_index == 1) {
                    load_all_sd_filenames_into_global();
                    move_to_history();
                }
            }

            break;

        case HEATING:

            if (handle_button_press(BACK)) {
                move_to_idle();
            }
            
            // target temp reached, start test
            if (check_conditions_for_test_start()) {  
                reset_test_data(); 
                move_to_reacting();
            }

            break;

        case REACTING:

            if (handle_button_press(BACK)) {
                move_to_idle();
            }

            if (check_test_completed()) {
                determine_results();
                int next_filename_int = get_latest_filename_int() + 1;
                char filename[256];
                snprintf(filename, sizeof(filename), "test_result_%d.json", next_filename_int);
                save_test_result_from_global_to_filename(filename);
                gSystemInfo.latest_filename_int = next_filename_int;
                save_metadata_to_sd_card();
                move_to_results();
            }

            tube_sens_ctrl_step();
            test_step();
            tube_optical_ctrl_step();
            
            break;

        case RESULTS:

            if (handle_button_press(SELECT)) {
                move_to_idle();
            }

            if (handle_button_press(BACK)) {
                move_to_history();
            }

            break;

        case HISTORY:
        
            if (!gButtonInput.wasPressed) {
                break;
            }

            gHistoryIM.needs_redraw = true;

            // TODO bruh

            if (handle_button_press(UP)) {
                if (gHistoryIM.selected_index == 0) {
                    break;
                }

                gHistoryIM.selected_index = gHistoryIM.selected_index - 1; 

                if (gHistoryIM.selected_index_relative == 0 && gHistoryIM.scroll_index > 0) {
                    gHistoryIM.scroll_index = gHistoryIM.scroll_index - 1;
                }
                else {
                    gHistoryIM.selected_index_relative = gHistoryIM.selected_index_relative - 1;
                }
            }
            
            if (handle_button_press(DOWN)) {
                if (gHistoryIM.selected_index == gHistoryIM.num_items - 1) {
                    break;
                }

                gHistoryIM.selected_index = gHistoryIM.selected_index + 1; 

                if (gHistoryIM.selected_index_relative == 3 && 
                    gHistoryIM.scroll_index < gHistoryIM.num_items - 4) {
                    gHistoryIM.scroll_index = gHistoryIM.scroll_index + 1;
                }
                else {
                    gHistoryIM.selected_index_relative = gHistoryIM.selected_index_relative + 1;
                }

            }
            
            if (handle_button_press(SELECT)) {

                int idx = gHistoryIM.selected_index;
                load_test_result_from_filename_into_global(results_menu_items[idx]);

                move_to_results();
            }

            if (handle_button_press(BACK)) {
                move_to_idle();
            }

            

    }

}