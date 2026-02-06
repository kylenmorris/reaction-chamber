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
#include "screens_interface.h"

void run_debug_loop() {
    draw_debug_screen();
}