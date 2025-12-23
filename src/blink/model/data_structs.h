#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdbool.h> 
#include "constants.h"
#include <stdint.h>

typedef struct {
    bool start_tube[NUM_TUBES];
} SystemControl;

extern SystemControl gSysControl;

// Button types
typedef enum {
    UP,
    DOWN,
    SELECT,
    BACK
} ButtonType;


// Tube states
typedef enum {
    EMPTY,
    RUNNING,
    POSITIVE,
    NEGATIVE,
    INVALID
} TubeState;


// // Tube results
// // may want to merge with TubeState
// typedef enum {
//     UNKNOWN,
//     POSITIVE,
//     NEGATIVE,
//     INVALID_RESULT 
// } TubeResult;

typedef struct {

    TubeState state;
    // TubeResult result;
    uint32_t start_time;
    uint32_t last_update;
    bool is_positive_control;
    bool is_negative_control;
    bool positive_detected;
    uint32_t ct_time;

} TubeReaction;


typedef struct {
    TubeReaction tubes[NUM_TUBES];
    uint32_t reaction_start_time;
    bool reaction_active;
    bool test_invalid;
    bool completed;
} TestStatus;

extern TestStatus gTestStatus;

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
    int intensity[NUM_TUBES];
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
