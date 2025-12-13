#include "../include/data_structs.h"

ButtonInput gButtonInput = {
    // .lastPressed = ,
    .lastPressed = -1,
    .wasPressed = false,
    .buttonStates = { HIGH, HIGH, HIGH, HIGH },
    .lastButtonStates = { HIGH, HIGH, HIGH, HIGH },
    .debounceCycles = { 0, 0, 0, 0 }
};

