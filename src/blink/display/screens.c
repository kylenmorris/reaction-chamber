#include "screens.h"

#include "data_structs.h"
#include "imodel_structs.h"
#include <stdio.h>

#include "glcd.h"

// ####################################
// HELPER PRINT FUNCTIONS
// ####################################

static char* get_tube_state_string(TubeState state) {
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

static char* get_result_string(ReactionResult result) {
    switch (result) {
        case UNKNOWN:
            return "UNKN";
        case POSITIVE:
            return "POS";
        case NEGATIVE:
            return "NEG";
        case INVALID_RESULT:
            return "ERR";
        default:
            return "__ERROR__";
    }
}

static void print_spinner(int frame) {
    const char spinner_chars[] = {'|', '/', '-', '\\'};
    printf("%c\r", spinner_chars[frame % 4]);
}

static void draw_progress_bar(int percent) {
    int filled = (percent * PROGRESS_BAR_WIDTH) / 100;

    printf("[");
    for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        if (i < filled)
            printf("=");
        else
            printf(" ");
    }
    printf("] %3d%%  ", percent);
}


// ####################################
// DISPLAY SCREENS
// ####################################

void glcd_draw_idle_menu(void) {

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

void glcd_draw_heating_screen(void) {
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

void glcd_draw_test_running_screen(void) {

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

void glcd_draw_results_screen(void) {

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


void glcd_draw_results_history(void) {
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


// ####################################
// TERMINAL SCREENS
// ####################################

#include <stdio.h>
void terminal_draw_idle_menu() {
    // Clear terminal (works on most terminals)
    printf("\033[2J");    // clear screen
    printf("\033[H");     // move cursor to top-left

    printf("================================\n");
    printf("        DEVICE IDLE MENU        \n");
    printf("================================\n\n");

    for (int i = 0; i < IDLE_MENU_ITEM_COUNT; i++) {
        if (i == gIdleMenuIM.selected_index) {
            printf(" > %s\n", idle_menu_items[i]);
        } else {
            printf("   %s\n", idle_menu_items[i]);
        }
    }

    printf("\n--------------------------------\n");
    printf("UP/DOWN: Navigate   SEL: Select\n");
    printf("================================\n");
}

void terminal_draw_heating_screen() {
    // Clear terminal
    printf("\033[2J");
    printf("\033[H");

    printf("========================================\n");
    printf("               HEATING MENU             \n");
    printf("========================================\n\n");

    // Show temperature
    printf("[%6.2f C / %6.2f C]\n",
           gTempStatus.chamber_temp, gTempStatus.target_temp);

    // Show progress bar
    draw_progress_bar(gTempStatus.chamber_temp * 100 / gTempStatus.target_temp);
    print_spinner(gHeatingMenuIM.spinner_frame);
    gHeatingMenuIM.spinner_frame = (gHeatingMenuIM.spinner_frame + 1) % 4; // cant just increment or we might have overflow lol

    printf("\n----------------------------------------\n");
    printf("BACK: Cancel Heating\n");
    printf("========================================\n");
}

void terminal_draw_test_running_screen() {
    // Clear terminal
    printf("\033[2J");
    printf("\033[H");

    printf("========================================\n");
    printf("           TEST RUNNING SCREEN          \n");
    printf("========================================\n\n");

    // Show test progress
    printf("Test in progress...\n");
    printf("Elapsed Time: [%dm %ds / %dm]\n", 
        (gTestStatus.reaction_total_time) / 60000, 
        (gTestStatus.reaction_total_time % 60000) / 1000,
        REACTION_DURATION_MS / 60000);

    // Show tube statuses
    for (int i = 0; i < NUM_TUBES / 2; i++) {
        printf("%-2d: %-10s | %-2d: %-10s\n",
               i + 1,
               get_tube_state_string(gTestStatus.tubes[i].state),
               i + NUM_TUBES / 2 + 1,
               get_tube_state_string(gTestStatus.tubes[i + NUM_TUBES / 2].state));
    }

    printf("\n----------------------------------------\n");
    printf("BACK: Abort Test\n");
    printf("========================================\n");
}

void terminal_draw_results_screen() {
    // Clear terminal
    printf("\033[2J");
    printf("\033[H");

    printf("========================================\n");
    printf("              RESULTS MENU              \n");
    printf("========================================\n\n");

    // Show test results
    for (int i = 0; i < NUM_TUBES / 2; i++) {
        printf("%-2d: %-10s | %-2d: %-10s\n",
               i + 1,
               get_result_string(gTestStatus.tubes[i].result),
               i + NUM_TUBES / 2 + 1,
               get_result_string(gTestStatus.tubes[i + NUM_TUBES / 2].result));
    }

    printf("\n----------------------------------------\n");
    printf("SELECT: Return to Idle Menu\n");
    printf("========================================\n");
}

void terminal_draw_results_history() {
    // Clear terminal
    printf("\033[2J");
    printf("\033[H");

    printf("========================================\n");
    printf("           RESULTS HISTORY MENU         \n");
    printf("========================================\n\n");

    // Placeholder for history display
    printf("No history available.\n");

    printf("\n----------------------------------------\n");
    printf("SELECT: Return to Idle Menu\n");
    printf("========================================\n");
}



// ####################################
// PUBLIC FUNCTIONS
// ####################################

void draw_idle_menu(void) {
    #if defined(DRAW_DISPLAY)
    glcd_draw_idle_menu();
    #endif
    #if defined(DRAW_TERMINAL)
    terminal_draw_idle_menu();
    #endif
}

void draw_heating_screen(void) {
    #if defined(DRAW_DISPLAY)
    glcd_draw_heating_screen();
    #endif
    #if defined(DRAW_TERMINAL)
    terminal_draw_heating_screen();
    #endif
}

void draw_test_running_screen(void) {
    #if defined(DRAW_DISPLAY)
    glcd_draw_test_running_screen();
    #endif
    #if defined(DRAW_TERMINAL)
    terminal_draw_test_running_screen();
    #endif
}

void draw_results_screen(void) {
    #if defined(DRAW_DISPLAY)
    glcd_draw_results_screen();
    #endif
    #if defined(DRAW_TERMINAL)
    terminal_draw_results_screen();
    #endif
}

void draw_results_history(void) {
    #if defined(DRAW_DISPLAY)
    glcd_draw_results_history();
    #endif
    #if defined(DRAW_TERMINAL)
    terminal_draw_results_history();
    #endif
}