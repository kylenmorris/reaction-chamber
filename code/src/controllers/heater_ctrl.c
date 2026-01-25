#include "heater_ctrl.h"

#include "constants.h"
#include "data_structs.h"
#include "drivers.h"

void heater_ctrl_step(void) {

    bool reaction_active = gTestStatus.reaction_active;
    bool heating_active = gTestStatus.heating_active;

    bool sys_state_valid = gSystemState != (HEATING || REACTING);
    bool temp_sensors_vald = (gTempStatus.chamber_temp > LOWEST_POSSIBLE_TEMP) 
                            && (gTempStatus.chamber_temp < TEMP_HIGH_HIGH_C);
    
    if (!(reaction_active || heating_active) || !sys_state_valid || !temp_sensors_vald) {
        hw_heater_toggle(false);
        return;        
    }

    if (gTempStatus.chamber_temp < TEMP_DESIRED_C - 0.5) {
        hw_heater_toggle(true);
    } 
    else {
        hw_heater_toggle(false);
    }

}

