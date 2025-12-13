#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdbool.h> 
#include "constants.h"

// Button types
typedef enum {
    UP,
    DOWN,
    SELECT,
    BACK
} ButtonType;

// Input state
typedef struct {
    // ButtonType lastPressed;
    int lastPressed;
    bool wasPressed;
    int buttonStates[NUM_BUTTONS];
    int lastButtonStates[NUM_BUTTONS];
    int debounceCycles[NUM_BUTTONS];
} ButtonInput;

// Declare the global variable (no memory yet)
extern ButtonInput gButtonInput;

#endif
