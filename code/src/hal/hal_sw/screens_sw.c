#include "screens_interface.h"
#include "data_structs.h"
#include "data_helpers.h"
#include <stdio.h>

static void print_spinner(int frame) {
    const char spinner_chars[] = {'|', '/', '-', '\\'};
    printf("%c\r", spinner_chars[frame % 4]);
}

static void draw_progress_bar(float percent) {
    int filled = (percent * PROGRESS_BAR_WIDTH) / 100;

    printf("[");
    for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        if (i < filled)
            printf("=");
        else
            printf(" ");
    }
    printf("] %3f%%  ", percent);
}

// ####################################
// TERMINAL SCREENS
// ####################################

void draw_loading_screen() {
    printf("\033[2J");    // clear screen
    printf("\033[H");     // move cursor to top-left
    printf("================================\n");
    printf("        DEVICE BUSY          \n");
    printf("================================\n\n");
    printf("Please wait...\n");
    printf("================================\n");
}

void draw_idle_screen() {
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

void draw_heating_screen() {
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

void draw_test_running_screen() {
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

void draw_results_screen() {
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

void draw_results_history() {
    // Clear terminal
    printf("\033[2J");
    printf("\033[H");

    printf("========================================\n");
    printf("           RESULTS HISTORY MENU         \n");
    printf("========================================\n\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (i == gHistoryIM.selected_index) {
            printf(" > %s\n", results_menu_items[i]);
        } else {
            printf("   %s\n", results_menu_items[i]);
        }
    }

    printf("\n----------------------------------------\n");
    printf("SELECT: Return to Idle Menu\n");
    printf("========================================\n");
}


