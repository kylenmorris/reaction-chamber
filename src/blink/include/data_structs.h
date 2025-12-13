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


// Tube states
typedef enum {
    UNUSED,
    RUNNING,
    COMPLETE,
    INVALID
} TubeState;

// Tube results
// may want to merge with TubeState
typedef enum {
    UNKNOWN,
    POSITIVE,
    NEGATIVE,
    INVALID_RESULT 
} TubeResult;


// Button input structure
// Written only by button_ctrl.c
typedef struct {
    int lastPressed;
    bool wasPressed;
    int buttonStates[NUM_BUTTONS];
    int lastButtonStates[NUM_BUTTONS];
    int debounceCycles[NUM_BUTTONS];
} ButtonInput;


// Tube optical readings
// Written only by tube_optical_ctrl.c
typedef struct {
    float intensity[NUM_TUBES];
} OpticalInputs;


// Temperature status
// Written only by temp_sens_ctrl.c
typedef struct {
    float chamber_temp; // °C
    bool temp_low;      // default false
    bool temp_extreme;  // default false
} TemperatureStatus;



extern ButtonInput gButtonInput;
extern TemperatureStatus gTemperatureStatus;
extern OpticalInputs gOpticalInputs;

#endif
