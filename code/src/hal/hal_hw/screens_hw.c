#include "constants.h"
#include "glcd_text_tiny.h"
#include "screens_interface.h"

#include "glcd.h"
#include "glcd_controllers.h"
#include "glcd_text.h"

#include "data_structs.h"
#include "data_helpers.h"
#include <stdio.h>


#ifdef USE_HW_DISPLAY

void display_error_banner() {
    
    glcd_clear();

    int x = 5;
    int y = 3;
    if (gSystemError.current_error != ERROR_NONE) {
        glcd_tiny_draw_string(x, y, "ERROR: ");
        glcd_tiny_draw_string(x, y + 1, get_error_string(gSystemError.current_error));
    }

    glcd_write();
}

// Print with minimal formatting to fit on screen
// All 4 temp sensors, 10 tube states, and 10 optical readings should fit on the screen
void draw_debug_screen(void) {
    glcd_clear();

    
    if (gDebugMenuIM.selected_index == 0) {
        glcd_tiny_draw_string(0, 0, "Temp Sensors:");
        for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
            char line[32];
            snprintf(line, sizeof(line), "%d: %.2f C", i, gTempStatus.temp_readings[i]);
            glcd_tiny_draw_string(0, i+1, line);
        }

        // draw heater state
        char heater_line[32];
        if (gHeaterState.heaterOn) {
            snprintf(heater_line, sizeof(heater_line), "Heater: ON");
        } else {
            snprintf(heater_line, sizeof(heater_line), "Heater: OFF");
        }
        // snprintf(heater_line, sizeof(heater_line), "Heater: %s", gHeaterState.heaterOn ? "ON" : "OFF");
        glcd_tiny_draw_string(0, NUM_TEMP_SENSORS + 2, heater_line);
    }

    if (gDebugMenuIM.selected_index == 1) {
        glcd_tiny_draw_string(0, 0, "Tube States:");

        for (int i = 0; i < NUM_TUBES / 2; i++) {
            char line[64];
            snprintf(line, sizeof(line), 
                    "%-1d:%-6s | %-2d:%-6s",
                    i + 1,
                    get_tube_state_string(gSysControl.tube_present[i]),
                    i + NUM_TUBES / 2 + 1,
                    get_tube_state_string(gSysControl.tube_present[i + NUM_TUBES / 2]));
            
            glcd_tiny_draw_string(0, 1 + i, line);
        }
    }

    if (gDebugMenuIM.selected_index == 2) {
        glcd_tiny_draw_string(0, 0, "Optical Inputs:");

        for (int i = 0; i < NUM_TUBES / 2; i++) {
            char line[64];
            snprintf(line, sizeof(line), 
                    "%-1d:%-4.0f %c | %-2d:%-4.0f %c",
                    i + 1,
                    gOpticalInputs.intensity[i],
                    (gOpticalInputs.intensity[i] > OPTICAL_REACTION_THRESHOLD) ? '*' : ' ',
                    i + NUM_TUBES / 2 + 1,
                    gOpticalInputs.intensity[i + NUM_TUBES / 2],
                    (gOpticalInputs.intensity[i + NUM_TUBES / 2] > OPTICAL_REACTION_THRESHOLD) ? '*' : ' '
                );
            
            glcd_tiny_draw_string(0, 1 + i, line);
        }

    }

    if (gDebugMenuIM.selected_index == 3) {
        glcd_tiny_draw_string(0, 0, "Tubes and Detections:");

        for (int i = 0; i < NUM_TUBES / 2; i++) {
            char line[64];

            if (gSysControl.tube_present[i] == false) {
                snprintf(line, sizeof(line), 
                    // "%-1d: ABSENT | %-2d: ABSENT",
                    "%-1d: ABSENT",
                    i + 1
                    );
            } 
            else {
                snprintf(line, sizeof(line), 
                        "%-1d: %-10s [%-4.0f] ",
                        i + 1,
                        (gOpticalInputs.intensity[i] > OPTICAL_REACTION_THRESHOLD) ? "DETECTED" : "NO DETECT",
                        gOpticalInputs.intensity[i]
                    );
            }
            glcd_tiny_draw_string(0, 1 + i, line);
        }        
    }

    if (gDebugMenuIM.selected_index == 4) {
        glcd_tiny_draw_string(0, 0, "LED Status:");
        if (gSystemInfo.led_state) {
            glcd_tiny_draw_string(0, 1, "LED is ON");
        } else {
            glcd_tiny_draw_string(0, 1, "LED is OFF");
        }
    }

    glcd_write();
}

void draw_idle_screen(void) {

    glcd_clear();

    glcd_tiny_draw_string(0, 0, "MAIN MENU");
    
    const int item_y_start = 1;
    const int item_spacing = 1;
    
    for (int i = 0; i < IDLE_MENU_ITEM_COUNT; i++) {
        int y = item_y_start + item_spacing * i;
        
        if (i == gIdleMenuIM.selected_index) {
            glcd_tiny_draw_string(5, y, ">");
        }
        
        glcd_tiny_draw_string(15, y, idle_menu_items[i]);
    }
        
    glcd_tiny_draw_string(0, 7, "SEL: Select");
    
    glcd_write();
}

void draw_heating_screen(void) {
    glcd_clear();

    glcd_tiny_draw_string(0, 0, "HEATING MENU");
    
    /* Show temperature */
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "[%.2f C / %.2f C]", 
             gTempStatus.chamber_temp, gTempStatus.target_temp);
    glcd_tiny_draw_string(5, 2, temp_str);
    
    int percent = (gTempStatus.chamber_temp * 100) / gTempStatus.target_temp;
    int filled = (percent * PROGRESS_BAR_WIDTH) / 100;
    char bar_str[PROGRESS_BAR_WIDTH + 3];

    bar_str[0] = '[';
    for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        bar_str[i + 1] = (i < filled) ? '=' : ' ';
    }
    bar_str[PROGRESS_BAR_WIDTH + 1] = ']';
    bar_str[PROGRESS_BAR_WIDTH + 2] = '\0';
    
    glcd_tiny_draw_string(5, 4, bar_str);
    
    glcd_tiny_draw_string(0, 7, "BACK: Cancel");
    
    glcd_write();
}

void draw_test_running_screen(void) {

    glcd_clear();

    char test_str[32];
    snprintf(test_str, sizeof(test_str), "TEST ACTIVE, LEDS %s", gSystemInfo.led_state ? "ON" : "OFF");
    glcd_tiny_draw_string(0, 0, test_str);
        
    /* Show test progress */
    char progress_str[64];
    snprintf(progress_str, sizeof(progress_str), 
             "[%dm %ds / %dm]", 
             (gTestStatus.reaction_total_time) / 60000, 
             (gTestStatus.reaction_total_time % 60000) / 1000,
             REACTION_DURATION_MS / 60000);
    glcd_tiny_draw_string(5, 1, progress_str);
    
    /* Show tube statuses */
    for (int i = 0; i < NUM_TUBES / 2; i++) {
        char tube_str[64];
        snprintf(tube_str, sizeof(tube_str), "%-1d:%-6s | %-2d:%-6s",
                 i + 1,
                 get_tube_state_string(gTestStatus.tubes[i].state),
                 i + NUM_TUBES / 2 + 1,
                 get_tube_state_string(gTestStatus.tubes[i + NUM_TUBES / 2].state));
        glcd_tiny_draw_string(0, 2 + i, tube_str);
    }

    glcd_write();
}

void draw_results_screen(void) {

    glcd_clear();

    glcd_tiny_draw_string(0, 0, "TEST RESULT");
    
    for (int i = 0; i < NUM_TUBES / 2; i++) {
        char result_str[64];
        snprintf(result_str, sizeof(result_str), 
                 "%-1d:%-6s | %-2d:%-6s",
                 i + 1,
                 get_result_string(gTestStatus.tubes[i].result),
                 i + NUM_TUBES / 2 + 1,
                 get_result_string(gTestStatus.tubes[i + NUM_TUBES / 2].result));
        
        glcd_tiny_draw_string(0, 1 + i, result_str);
    }
    
    glcd_tiny_draw_string(0, 7, "SEL: Okay");
    
    glcd_write();
}


void draw_results_history(void) {

    glcd_clear();

    glcd_tiny_draw_string(0, 0, "RESULTS HISTORY");

    int start = gHistoryIM.scroll_index;
    int end = start + 5;
    int sel_idx = gHistoryIM.selected_index_relative;

    for (int i = start; i < end; i++) {

        int y = (i + 1) - start; // Y position on screen (1-4)
        
        if ((i - start) == sel_idx) {
            glcd_tiny_draw_string(5, y, ">");
        }
        
        glcd_tiny_draw_string(15, y, results_menu_items[i]);
    }

    if (gHistoryIM.num_items > 5) {
        glcd_tiny_draw_string(5, 6, "Scroll: UP/DOWN");
    }

    glcd_tiny_draw_string(0, 7, "SEL: View  BACK: Main");
    
    glcd_write();
}

void draw_loading_screen(void) {
    glcd_clear();

    glcd_tiny_draw_string(0, 0, "Device busy...");

    glcd_write();
}

#endif