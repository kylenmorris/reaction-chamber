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
#include "screens_interface.h"

void run_debug_loop() {
    temp_sens_ctrl_step();
    button_ctrl_step();
    heater_ctrl_step();
    tube_optical_ctrl_step();
    tube_sens_ctrl_step();

    draw_debug_screen();

    if (handle_button_press(BACK)) {
        for (int i = 0; i < 4; i++) {
            char filename[32];
            snprintf(filename, sizeof(filename), "test_result_%d.json", i + 1);
            save_test_result_from_global_to_filename(filename);
        }
    } 
}