#include "constants.h"
#include "data_structs.h"
#include <stdio.h>
#include <stdlib.h>

#include "tube_sens_ctrl.h"
#include "button_ctrl.h"
#include "drivers.h"


void tube_sens_ctrl_init(void) {
    // // Initialize tube sensors here
    // for (int i = 0; i < NUM_TUBES; i++) {
    //     used_tubes[i] = false;    
    //     gTestStatus.tubes[i].state = EMPTY;
    // }


    // Reset tube statuses - potentially
    // for (int i = 0; i < NUM_TUBES; i++) {
    //     gTestStatus.tubes[i].state = EMPTY;
    //     gTestStatus.tubes[i].result = UNKNOWN;
    //     gTestStatus.tubes[i].start_time = 0;
    //     gTestStatus.tubes[i].last_update = 0;
    //     gTestStatus.tubes[i].is_positive_control = false;
    //     gTestStatus.tubes[i].is_negative_control = false;
    //     gTestStatus.tubes[i].positive_detected = false;
    //     gTestStatus.tubes[i].detection_time = 0;
    // }

}

// Detects the insertion of tubes and might do other stuff later
void tube_sens_ctrl_step(void) {

    uint16_t tubes_binary_0 = hw_tube_sens_read_all(SPI1_CS_SHIFT_SWITCHES_SR_PIN_0);
    uint16_t tubes_binary_1 = hw_tube_sens_read_all(SPI1_CS_SHIFT_SWITCHES_SR_PIN_1);

    // tubes_0 for tubes 1-5, tubes_1 for tubes 6-10
    for (int i = 0; i < NUM_TUBES; i++) {
        bool present = (i < 5) ? (tubes_binary_0 & (1 << i)) : (tubes_binary_1 & (1 << (i - 5)));
        gSysControl.tube_present[i] = present;
    }

}