#ifndef IMODEL_STRUCTS_H
#define IMODEL_STRUCTS_H

#include <stdbool.h>
#include <stdint.h>

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
    uint32_t last_redraw;
    bool needs_redraw;
} idleMenuIM;

extern idleMenuIM gIdleMenuIM;

// ####################################
// HEATING MENU
// ####################################

typedef struct {
    bool needs_redraw;
    uint32_t last_redraw;
} heatingMenuIM;

extern heatingMenuIM gHeatingMenuIM;




#endif
