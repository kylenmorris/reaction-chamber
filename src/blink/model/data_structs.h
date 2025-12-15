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


// System states for main loop
// Written only by system_state_loop.c
typedef enum {
    IDLE,
    HEATING,
    REACTING,
    RESULTS,
    HISTORY
} SystemState;

extern SystemState gSystemState;


// Button input structure
// Written only by button_ctrl.c
typedef struct {
    int lastPressed;
    bool wasPressed;
    int buttonStates[NUM_BUTTONS];
    int lastButtonStates[NUM_BUTTONS];
    int steadyTime[NUM_BUTTONS];
} ButtonInput;

extern ButtonInput gButtonInput;


// Tube optical readings
// Written only by tube_optical_ctrl.c
typedef struct {
    float intensity[NUM_TUBES];
} OpticalInputs;

extern OpticalInputs gOpticalInputs;


// Temperature status
// Written only by temp_sens_ctrl.c
typedef struct {
    float chamber_temp; // °C
    bool temp_low;      // default false
    bool temp_extreme;  // default false
    float target_temp;  // °C
} TempStatus;

extern TempStatus gTempStatus;

#endif
