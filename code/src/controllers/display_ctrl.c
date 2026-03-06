#include "display_ctrl.h"

#include "screens_interface.h"
#include "drivers.h"

#include "data_structs.h"
#include "constants.h"
#include <stdio.h>

// This might need a rework
static void redraw_if_needed(uint32_t now_ms, bool* needs_redraw, 
                            uint32_t* last_redraw, void (*draw_fn)(void)) {

// draw_fn();
    if ((now_ms - *last_redraw) >= REDRAW_INTERVAL_MS || *needs_redraw) {
        draw_fn();
        *last_redraw = now_ms;
        *needs_redraw = false;
    }
    
}

// ####################################
// PUBLIC METHODS
// ####################################

// Draws the currently relevant screen based on gSystemState and gSystemError 
void display_ctrl_step() {
    
    uint32_t now_ms = get_current_time();
    // printf("Displaying screen for state: %d\n", gSystemState);

    if (gSystemError.current_error != ERROR_NONE) {
        // printf("Current error: %d\n", gSystemError.current_error);
        redraw_if_needed(now_ms, &gErrorMenuIM.needs_redraw, 
                &gErrorMenuIM.last_redraw, display_error_banner);
        return;
    }

    switch (gSystemState) {
        case IDLE:
            redraw_if_needed(now_ms, &gIdleMenuIM.needs_redraw, 
                    &gIdleMenuIM.last_redraw, draw_idle_screen);
            break;

        case HEATING:
            redraw_if_needed(now_ms, &gHeatingMenuIM.needs_redraw, 
                    &gHeatingMenuIM.last_redraw, draw_heating_screen);
            break;

        case REACTING:
            redraw_if_needed(now_ms, &gTestRunningIM.needs_redraw, 
                    &gTestRunningIM.last_redraw, draw_test_running_screen);
            break;

        case RESULTS:
            redraw_if_needed(now_ms, &gResultsIM.needs_redraw, 
                    &gResultsIM.last_redraw, draw_results_screen);
            break;

        case HISTORY:
            redraw_if_needed(now_ms, &gHistoryIM.needs_redraw, 
                    &gHistoryIM.last_redraw, draw_results_history);
            break;
            
        case BOOT:
            draw_loading_screen();
            break;

        case DEBUG:
            redraw_if_needed(now_ms, &gDebugMenuIM.needs_redraw, 
                    &gDebugMenuIM.last_redraw, draw_debug_screen);
            break;

        }
}
