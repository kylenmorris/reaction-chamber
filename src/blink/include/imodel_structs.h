#ifndef IMODEL_STRUCTS_H
#define IMODEL_STRUCTS_H

#include <stdbool.h>

#define PROGRESS_BAR_WIDTH 20
#define IDLE_MENU_ITEM_COUNT 2 // might be excessive to define this

// ####################################
// IDLE MENU
// ####################################

static const char *idle_menu_items[] = {
    "Start Test",
    "View History"
};

typedef struct {
    int selected_index;
    bool needs_redraw;
} menu_model_t;

extern menu_model_t gIdleMenuModel;

// ####################################
// HEATING MENU
// ####################################




#endif
