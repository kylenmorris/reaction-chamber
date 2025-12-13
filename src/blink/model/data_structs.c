#include "../include/data_structs.h"

ButtonInput gButtonInput = {
    // .lastPressed = ,
    .lastPressed = -1,
    .wasPressed = false,
    .buttonStates = { HIGH, HIGH, HIGH, HIGH },
    .lastButtonStates = { HIGH, HIGH, HIGH, HIGH },
    .steadyTime = { 0, 0, 0, 0 }
};

TempStatus gTempStatus = {
    .chamber_temp = 0.0f,
    .temp_low = false,
    .temp_extreme = false
};

OpticalInputs gOpticalInputs = {
    .intensity = { 0.0f }
};

