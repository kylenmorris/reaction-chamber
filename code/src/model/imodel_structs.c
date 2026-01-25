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
    .last_redraw = 0
};

historyIM gHistoryIM = {
    .needs_redraw = true,
    .last_redraw = 0
};