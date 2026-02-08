#include "data_structs.h"
#include <stdint.h>

// ####################################
// STRUCTS
// ####################################

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
    .ambient_temp = 0.0f,
    .heat_rate = 0.1f,
    .cool_rate = 0.1f,
    .force_temp_sensor_fault = false
};

uint16_t gSimTime = 10;

// ####################################
// IMODEL STRUCTS
// ####################################

idleMenuIM gIdleMenuIM = {
    .selected_index = 0,
    .needs_redraw = true,
    .last_redraw = 0
};

heatingMenuIM gHeatingMenuIM = {
    .needs_redraw = true,
    .last_redraw = 0
};

testRunningIM gTestRunningIM = {
    .needs_redraw = true,
    .last_redraw = 0
};

resultsIM gResultsIM = {
    .needs_redraw = true,
    .last_redraw = 0,
};

// Populated by sd_drv.c
char results_menu_items[MAX_FILES][MAX_NAME_LEN];

historyIM gHistoryIM = {
    .needs_redraw = true,
    .last_redraw = 0,
    .selected_index = 0,
    .screen_scroll_index = 0,
    .selected_index_relative = 0,
    .num_items = 0
};

bool filenames_need_update = true;