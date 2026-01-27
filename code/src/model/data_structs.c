#include "data_structs.h"
#include <stdint.h>

// See header for info

SystemState gSystemState = BOOT;

SystemControl gSysControl = {
    .tube_present = { false }
};

TestStatus gTestStatus = {
    // .tubes = { {EMPTY, 0, 0, false, false, false, 0} },
    .reaction_start_time = 0,
    .reaction_total_time = 0,
    .reaction_active = false,
    .test_invalid = false,
    .completed = false
};

HeaterState gHeaterState = {
    .heaterOn = false
};

ButtonInput gButtonInput = {
    .lastPressed = -1,
    .wasPressed = false,
    .buttonStates = { HIGH, HIGH, HIGH, HIGH },
    .lastButtonStates = { HIGH, HIGH, HIGH, HIGH },
    .steadyTime = { 0, 0, 0, 0 }
};

OpticalInputs gOpticalInputs = {
    .intensity = { 0.0f }
};

TempStatus gTempStatus = {
    .chamber_temp = 0.0f,
    .temp_low = false,
    .temp_extreme = false,
    .target_temp = 62.0f
};

SimParams gSimParams = {

};

uint16_t gSimTime = 10;

void reset_sim_params() {
    gSimParams.ambient_temp = 0.0f;
    gSimParams.heat_rate = 0.1f;
    gSimParams.cool_rate = 0.1f;
    gSimParams.force_temp_sensor_fault = false;
}

char* get_system_state_string(SystemState state) {
    switch (state) {
        case IDLE:    return "IDLE";
        case HEATING: return "HEATING";
        case REACTING: return "REACTING";
        case RESULTS:  return "RESULTS";
        case HISTORY:  return "HISTORY";
        case BOOT:     return "BOOT";
        default:       return "UNKNOWN_STATE";
    }
}

char* get_tube_state_string(TubeState state) {
    switch (state) {
        case EMPTY:
            return "EMPTY";
        case RUNNING:
            return "RUN";
        case COMPLETED:
            return "FINISH";
        case ERROR:
            return "ERROR";
        default:
            return "__ERROR__";
    }
}

char* get_result_string(ReactionResult result) {
    switch (result) {
        // case UNKNOWN:
        //     return "UNKN";
        case POSITIVE:
            return "POS";
        case NEGATIVE:
            return "NEG";
        case INVALID_RESULT:
            return "INVALD";
        default:
            return "ERR";
    }
}

// Check if a specific button was pressed
bool button_is_pressed(ButtonType button) {
    return gButtonInput.wasPressed && gButtonInput.lastPressed == button;
}