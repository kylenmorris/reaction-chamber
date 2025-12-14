#ifndef IMODEL_STRUCTS_H
#define IMODEL_STRUCTS_H

#include <stdbool.h>

#define IDLE_MENU_ITEM_COUNT 2

#define PROGRESS_BAR_WIDTH 20

static const char *idle_menu_items[] = {
    "Start Test",
    "View History"
};

typedef struct {
    int selected_index;   // 0 or 1
    bool needs_redraw;
} menu_model_t;



extern menu_model_t gIdleMenuModel;

#endif
