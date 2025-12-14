#include <stdio.h>
#include "display_ctrl.h"
#include "data_structs.h"
#include "imodel_structs.h"
#include "pico/time.h"

static void draw_progress_bar(int percent) {
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

static void terminal_draw_heating_screen() {
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

static void terminal_draw_idle_menu() {
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

// ####################################
// PUBLIC METHODS
// ####################################

void display_ctrl_draw(SystemState state) {
    
    uint32_t now_ms = to_ms_since_boot(get_absolute_time());

    switch (state) {
        case IDLE:
            if (gIdleMenuIM.needs_redraw) {
                terminal_draw_idle_menu();
                gIdleMenuIM.needs_redraw = false;
            }
            break;

        case HEATING:
            if ((now_ms - gHeatingMenuIM.last_redraw) >= REDRAW_INTERVAL_MS
                    || gHeatingMenuIM.needs_redraw) {
                terminal_draw_heating_screen();
                gHeatingMenuIM.last_redraw = now_ms;
                gHeatingMenuIM.needs_redraw = false;
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
