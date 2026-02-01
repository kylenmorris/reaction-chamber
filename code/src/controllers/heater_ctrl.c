#include "heater_ctrl.h"

#include "constants.h"
#include "data_structs.h"
#include "drivers.h"
#include <stdio.h>

// Reads from gSystemState and gTempStatus to control the heater
void heater_ctrl_step(void) {

    bool sys_state_invalid = !(gSystemState == HEATING || gSystemState == REACTING);
    bool temp_sensors_invald = (gTempStatus.chamber_temp < LOWEST_POSSIBLE_TEMP) 
                                || (gTempStatus.chamber_temp > HIGHEST_POSSIBLE_TEMP);
    
    if (sys_state_invalid || temp_sensors_invald) {
        hw_heater_toggle(false);
        gHeaterState.heaterOn = false;
        return;        
    }

    // If the heater is on, let it rise to 0.3 above desired. If it's off,
    // let it fall to 0.3 below. This prevents rapid switching of the heater
    if (gHeaterState.heaterOn && (gTempStatus.chamber_temp > (TEMP_DESIRED_C + 0.3))) {
        hw_heater_toggle(false);
        gHeaterState.heaterOn = false;
    }
    else if (!gHeaterState.heaterOn && (gTempStatus.chamber_temp < (TEMP_DESIRED_C - 0.3))) {
        hw_heater_toggle(true);
        gHeaterState.heaterOn = true;
    }

}

