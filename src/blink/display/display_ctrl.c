#include <stdio.h>
#include "display_ctrl.h"
#include "data_structs.h"
#include "imodel_structs.h"
#include "pico/time.h"
#include "constants.h"
#include "screens.h"

// This might need a rework
static void redraw_if_needed(uint32_t now_ms, bool* needs_redraw, 
                            uint32_t* last_redraw, void (*draw_fn)(void)) {

    if ((now_ms - *last_redraw) >= REDRAW_INTERVAL_MS || *needs_redraw) {
        draw_fn();
        *last_redraw = now_ms;
        *needs_redraw = false;
    }
    
}

// ####################################
// PUBLIC METHODS
// ####################################

void display_ctrl_draw() {
    
    uint32_t now_ms = to_ms_since_boot(get_absolute_time());

    switch (gSystemState) {
        case IDLE:
            if (gIdleMenuIM.needs_redraw) {
                terminal_draw_idle_menu();
                gIdleMenuIM.needs_redraw = false;
            }
            break;

        case HEATING:
            redraw_if_needed(now_ms, &gHeatingMenuIM.needs_redraw, 
                            &gHeatingMenuIM.last_redraw, terminal_draw_heating_screen);
            break;

        case REACTING:
            redraw_if_needed(now_ms, &gTestRunningIM.needs_redraw, 
                            &gTestRunningIM.last_redraw, terminal_draw_test_running_screen);
            break;

        case RESULTS:
            if (gResultsIM.needs_redraw) {
                terminal_draw_results_screen();
                gResultsIM.needs_redraw = false;
            }
            break;

        case HISTORY:

            break;
    }
}
