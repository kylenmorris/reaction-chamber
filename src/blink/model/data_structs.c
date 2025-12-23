#include "data_structs.h"

// See header for info

SystemState gSystemState = IDLE;

SystemControl gSysControl = {
    .start_tube = { false }
};

TestStatus gTestStatus = {
    // .tubes = { {EMPTY, 0, 0, false, false, false, 0} },
    .reaction_start_time = 0,
    .reaction_active = false,
    .test_invalid = false,
    .completed = false
};

ButtonInput gButtonInput = {
    // .lastPressed = ,
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


