#include "imodel_structs.h"
#include <stdbool.h>

idleMenuIM gIdleMenuIM = {
    .selected_index = 0,
    .needs_redraw = true,
    .last_redraw = 0
};

heatingMenuIM gHeatingMenuIM = {
    .needs_redraw = true,
    .last_redraw = 0
};

testRunningIM gTestRunningIM = {
    .needs_redraw = true,
    .last_redraw = 0
};

resultsIM gResultsIM = {
    .needs_redraw = true,
    .last_redraw = 0,
};

char results_menu_items[MAX_FILES][MAX_NAME_LEN];

historyIM gHistoryIM = {
    .needs_redraw = true,
    .last_redraw = 0,
    .selected_index = 0
};