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
