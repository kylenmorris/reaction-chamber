#include <stdio.h>

#include "data_structs.h"
#include "imodel_structs.h"


void draw_progress_bar(int percent) {
    int filled = (percent * PROGRESS_BAR_WIDTH) / 100;

    printf("[");
    for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        if (i < filled)
            printf("=");
        else
            printf(" ");
    }
    printf("] %3d%%\n", percent);
}

#include <stdio.h>

void terminal_draw_heating_screen() {
    // Clear terminal
    printf("\033[2J");
    printf("\033[H");

    printf("========================================\n");
    printf("           HEATING / TEST MENU          \n");
    printf("========================================\n\n");

    // Show temperature
    printf("Current Temp: %6.2f C   Target Temp: %6.2f C\n",
           gTempStatus.chamber_temp, gTempStatus.target_temp);

    // Show progress bar
    draw_progress_bar(gTempStatus.chamber_temp * 100 / gTempStatus.target_temp);

    printf("\n----------------------------------------\n");
    printf("BACK: Cancel Heating\n");
    printf("========================================\n");
}



void terminal_draw_idle_menu() {
    // Clear terminal (works on most terminals)
    printf("\033[2J");    // clear screen
    printf("\033[H");     // move cursor to top-left

    printf("================================\n");
    printf("        DEVICE IDLE MENU        \n");
    printf("================================\n\n");

    for (int i = 0; i < IDLE_MENU_ITEM_COUNT; i++) {
        if (i == gIdleMenuModel.selected_index) {
            printf(" > %s\n", idle_menu_items[i]);
        } else {
            printf("   %s\n", idle_menu_items[i]);
        }
    }

    printf("\n--------------------------------\n");
    printf("UP/DOWN: Navigate   SEL: Select\n");
    printf("================================\n");
}

void draw_display(SystemState state) {
    switch (state) {
        case IDLE:
            if (gIdleMenuModel.needs_redraw) {
                terminal_draw_idle_menu();
                // gIdleMenuModel.needs_redraw = false;
            }
            break;

        case HEATING:
            if (gIdleMenuModel.needs_redraw) {
                terminal_draw_heating_screen();
                // gIdleMenuModel.needs_redraw = false;
            }
            break;

        case REACTING:

            break;

        case RESULTS:

            break;

        case HISTORY:

            break;
    }
}

/*

    printf("\n----------------------------------------\n");
    printf(" Tube | State     | Value\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < NUM_TUBES; i++) {
        const tube_model_t *t = &test->tubes[i];

        if (t->valid) {
            printf("  %2d  | %s | %6.2f\n",
                   i,
                   tube_state_str(t->state),
                   t->value);
        } else {
            printf("  %2d  | %s |   ---\n",
                   i,
                   tube_state_str(t->state));
        }
    }

    */