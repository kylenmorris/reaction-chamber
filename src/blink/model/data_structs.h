#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdbool.h> 
#include "constants.h"
#include <stdint.h>

// ####################################
// ENUMS
// ####################################

// Button types
typedef enum {
    UP,
    DOWN,
    SELECT,
    BACK,
    DEBUG
} ButtonType;

// Physical tube state
typedef enum {
    EMPTY,
    RUNNING,
    COMPLETED,
    ERROR
} TubeState;

// Reaction results
typedef enum {
    UNKNOWN,
    POSITIVE,
    NEGATIVE,
    INVALID_RESULT 
} ReactionResult;


// ####################################
// STRUCTS
// ####################################

// monitors which tubes are inserted
// Written only by tube_sens_ctrl.c
typedef struct {
    bool tube_present[NUM_TUBES];
} SystemControl;

extern SystemControl gSysControl;


// Per tube info
// Written only by test_manager.c
typedef struct {

    TubeState state;
    ReactionResult result;
    uint32_t start_time;
    uint32_t last_update;
    bool is_positive_control;
    bool is_negative_control;
    bool positive_detected;
    uint32_t ct_time;

} TubeReaction;

// Test status
// Written only by test_manager.c
typedef struct {
    TubeReaction tubes[NUM_TUBES];
    uint32_t reaction_start_time;
    uint32_t reaction_total_time;
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
    HISTORY,
    BOOT
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
    bool target_reached;
} TempStatus;

extern TempStatus gTempStatus;

#endif
