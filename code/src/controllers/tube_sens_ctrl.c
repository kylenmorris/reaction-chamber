#include "constants.h"
#include "data_structs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tube_sens_ctrl.h"
#include "button_ctrl.h"
#include "drivers.h"


// Detects the insertion of tubes and might do other stuff later
void tube_sens_ctrl_step(void) {

    uint8_t tubes_binary_0 = hw_tube_sens_read_all(SPI1_CS_SHIFT_SWITCHES_SR_PIN_1);
    uint8_t tubes_binary_1 = hw_tube_sens_read_all(SPI1_CS_SHIFT_SWITCHES_SR_PIN_0);

    // printf("Tube sensor raw readings: 0x%02X 0x%02X\n", tubes_binary_0, tubes_binary_1);

    // pcb wiring is messed up. sue me 
    gSysControl.tube_present[0] = (tubes_binary_0 >> 3) & 1; 
    gSysControl.tube_present[1] = (tubes_binary_0 >> 2) & 1;
    gSysControl.tube_present[2] = (tubes_binary_0 >> 1) & 1; 
    gSysControl.tube_present[3] = (tubes_binary_0 >> 0) & 1;
    gSysControl.tube_present[4] = (tubes_binary_0 >> 4) & 1; 

    gSysControl.tube_present[5] = (tubes_binary_1 >> 0) & 1;
    gSysControl.tube_present[6] = (tubes_binary_1 >> 1) & 1;
    gSysControl.tube_present[7] = (tubes_binary_1 >> 2) & 1;
    gSysControl.tube_present[8] = (tubes_binary_1 >> 3) & 1;
    gSysControl.tube_present[9] = (tubes_binary_1 >> 4) & 1;
}