#include "data_helpers.h"

#include "data_structs.h"
#include <stdint.h>
#include <string.h>

void temp_sensor_reading_tracking(bool successful_read) {
    if (successful_read) {
        
    }
}

void reset_sim_params() {
    gSimParams.ambient_temp = 0.0f;
    gSimParams.heat_rate = 0.5f;
    gSimParams.cool_rate = 0.3f;
    gSimParams.force_temp_sensor_fault = false;
}

char* get_error_string(ErrorCode error) {
    switch (error) {
        case ERROR_SD_READ_FAILED:
            return "SD card read failed";
        case ERROR_SD_WRITE_FAILED:
            return "SD card write failed";
        case ERROR_TEST_INVALID:
            return "Test invalid. Check tubes.";
        case ERROR_TEMP_SENSOR_FAULT:
            return "Temperature sensor fault.";
        case ERROR_OPTICAL_SENSOR_FAULT:
            return "Optical sensor fault.";
        case ERROR_HEATER_FAULT:
            return "Heater fault.";
        case ERROR_NONE:
            return "No error";
        default:
            return "Unknown error";
        
    }
}

// Helper to map string results back to enums
ReactionResult string_to_result(const char* str) {
    if (strcmp(str, "POS") == 0) return POSITIVE;
    if (strcmp(str, "NEG") == 0) return NEGATIVE;
    return INVALID_RESULT;
}

// Helper to map string states back to enums
TubeState string_to_state(const char* str) {
    if (strcmp(str, "EMPTY") == 0) return EMPTY;
    if (strcmp(str, "RUNNING") == 0) return RUNNING;
    if (strcmp(str, "COMPLETED") == 0) return COMPLETED;
    return ERROR;
}

char* get_system_state_string(SystemState state) {
    switch (state) {
        case IDLE:    return "IDLE";
        case HEATING: return "HEATING";
        case REACTING: return "REACTING";
        case RESULTS:  return "RESULTS";
        case HISTORY:  return "HISTORY";
        case BOOT:     return "BOOT";
        default:       return "UNKNOWN_STATE";
    }
}

char* get_tube_state_string(TubeState state) {
    switch (state) {
        case EMPTY:
            return "EMPTY";
        case RUNNING:
            return "RUN";
        case COMPLETED:
            return "FINISH";
        case ERROR:
            return "ERROR";
        default:
            return "__ERROR__";
    }
}

char* get_result_string(ReactionResult result) {
    switch (result) {
        // case UNKNOWN:
        //     return "UNKN";
        case POSITIVE:
            return "POS";
        case NEGATIVE:
            return "NEG";
        case INVALID_RESULT:
            return "INVALD";
        default:
            return "ERR";
    }
}

// Check if a specific button was pressed
bool button_is_pressed(ButtonType button) {
    return gButtonInput.wasPressed && gButtonInput.lastPressed == button;
}

bool handle_button_press(ButtonType button) {
    if (button_is_pressed(button)) {
        gButtonInput.wasPressed = false;  // Reset flag
        return true;
    }
    return false;
}

