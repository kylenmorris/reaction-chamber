#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdbool.h> 
#include "constants.h"
#include <stdint.h>

extern bool DEBUG_MODE;

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

typedef enum {
    ERROR_NONE,
    ERROR_SD_READ_FAILED,
    ERROR_SD_WRITE_FAILED,
    ERROR_TEMP_SENSOR_FAULT,
    ERROR_TEST_INVALID,
    ERROR_OPTICAL_SENSOR_FAULT,
    ERROR_HEATER_FAULT
} ErrorCode;

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

typedef enum {
    TEST_IDLE,
    TEST_PREPARE,
    TEST_RUNNING,
    TEST_FINISHED
} TestManagerState;

extern TestManagerState gTestManagerState;

// ####################################
// STRUCTS
// ####################################

typedef struct {
    int temp_sensor_success_count; // can be positive or negative
    int optical_sensor_success_count; // can be positive or negative
    int latest_filename_int; // for generating new filenames
} SystemInfo;

extern SystemInfo gSystemInfo;

typedef struct {
    ErrorCode current_error;
    uint32_t error_timestamp;
    bool needs_display;  // Force redraw when error occurs
} SystemError;

extern SystemError gSystemError;

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
    uint32_t detection_time;

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

typedef struct {
    bool heaterOn;
} HeaterState;

extern HeaterState gHeaterState;

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

// simulation_engine.h
typedef struct {
    float ambient_temp;
    float heat_rate;      // degC per loop cycle
    float cool_rate;      // degC per loop cycle
    bool force_temp_sensor_fault;
} SimParams;

extern SimParams gSimParams;
extern uint16_t gSimTime;

extern bool filenames_need_update;

#define PROGRESS_BAR_WIDTH 16
#define IDLE_MENU_ITEM_COUNT 2 // might be excessive to define this

// ####################################
// IMODEL STRUCTS
// ####################################

typedef struct {
    int selected_index;
    uint32_t last_redraw;
    bool needs_redraw;
} ErrorMenuIM;

extern ErrorMenuIM gErrorMenuIM;

static char *idle_menu_items[] = {
    "Start Test",
    "View History"
};

typedef struct {
    int selected_index;
    uint32_t last_redraw;
    bool needs_redraw;
} idleMenuIM;

extern idleMenuIM gIdleMenuIM;

typedef struct {
    bool needs_redraw;
    uint32_t last_redraw;
    int spinner_frame;
} heatingMenuIM;

extern heatingMenuIM gHeatingMenuIM;

typedef struct {
    bool needs_redraw;
    uint32_t last_redraw;
} testRunningIM;

extern testRunningIM gTestRunningIM;

typedef struct {
    bool needs_redraw;
    uint32_t last_redraw;
} resultsIM;

extern resultsIM gResultsIM;

typedef struct {
    bool needs_redraw;
    uint32_t last_redraw;
    int selected_index; // absolute index of selection in file list
    int scroll_index; // how many lines have we scrolled down
    int selected_index_relative; // 0-3
    int num_items;
} historyIM;

extern historyIM gHistoryIM;

extern char results_menu_items[MAX_FILES][MAX_NAME_LEN];


#endif
