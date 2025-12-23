#include <stdio.h>
#include "display_ctrl.h"
#include "data_structs.h"
#include "imodel_structs.h"
#include "pico/time.h"
#include "constants.h"
#include "screens.h"

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
            if ((now_ms - gHeatingMenuIM.last_redraw) >= REDRAW_INTERVAL_MS
                    || gHeatingMenuIM.needs_redraw) {
                terminal_draw_heating_screen();
                gHeatingMenuIM.last_redraw = now_ms;
                gHeatingMenuIM.needs_redraw = false;
            }
            break;

        case REACTING:
            if ((now_ms - gTestRunningIM.last_redraw) >= REDRAW_INTERVAL_MS
                    || gTestRunningIM.needs_redraw) {
                terminal_draw_test_running_screen();
                gTestRunningIM.last_redraw = now_ms;
                gTestRunningIM.needs_redraw = false;
            }
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
