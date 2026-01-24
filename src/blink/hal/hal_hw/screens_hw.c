#include "glcd_controllers.h"
#include "glcd_text.h"
#include "screens_interface.h"

#include "data_structs.h"
#include "imodel_structs.h"
#include <stdio.h>

#include "glcd.h"

void hw_draw_idle_menu(void) {

    glcd_clear();
    
    glcd_tiny_draw_string(0, 0, "IDLE MENU");
    
    const int item_y_start = 1;
    const int item_spacing = 1;
    
    for (int i = 0; i < IDLE_MENU_ITEM_COUNT; i++) {
        int y = item_y_start + item_spacing * i;
        
        /* Draw selection indicator (arrow) */
        if (i == gIdleMenuIM.selected_index) {
            glcd_tiny_draw_string(5, y, ">");
        }
        
        /* Draw menu item text */
        glcd_tiny_draw_string(15, y, idle_menu_items[i]);
    }
        
    /* Draw bottom instruction bar */
    glcd_tiny_draw_string(0, 7, "UP/DOWN: Nav SEL: Sel");
    
    /* Update the display with changes */
    glcd_write();
}

void hw_draw_heating_screen(void) {
    glcd_clear();
    
    glcd_tiny_draw_string(0, 0, "HEATING MENU");
    
    /* Show temperature */
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "[%.2f C / %.2f C]", 
             gTempStatus.chamber_temp, gTempStatus.target_temp);
    glcd_tiny_draw_string(5, 2, temp_str);
    
    /* Draw progress bar */
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
    
    /* Draw bottom instruction bar */
    glcd_tiny_draw_string(0, 7, "BACK: Cancel Heating");
    
    glcd_write();
}

void hw_draw_test_running_screen(void) {

    glcd_clear();

    glcd_draw_string_xy(0, 0, "TEST RUNNING");
        
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

void hw_draw_results_screen(void) {

    glcd_clear();

    glcd_tiny_draw_string(0, 0, "RESULTS MENU");
    
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


void hw_draw_results_history(void) {
    /* Clear the screen buffer */
    glcd_clear();
    
    /* Draw top border line */
    glcd_draw_line(0, 8, 128, 8, BLACK);
    
    /* Title: "RESULTS HISTORY MENU" */
    glcd_draw_string_xy(10, 0, "RESULTS HISTORY MENU");
    
    /* Draw line below title */
    glcd_draw_line(0, 25, 128, 25, BLACK);
    
    /* Placeholder for history display */
    glcd_tiny_draw_string(0, 0, "RESULTS HISTORY");
    glcd_tiny_draw_string(0, 3, "No history available.");
    
    /* Draw bottom instruction bar */
    glcd_tiny_draw_string(0, 7, "SELECT: Return to Idle");
    
    glcd_write();
}

void hw_draw_boot_screen(void) {
    glcd_clear();

    glcd_draw_string_xy(0, 0, "PathoScan Booting...");

    glcd_write();
}
