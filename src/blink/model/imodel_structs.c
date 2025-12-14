#include "imodel_structs.h"
#include <stdbool.h>

idleMenuIM gIdleMenuIM = {
    .selected_index = 0,
    .needs_redraw = true
};

heatingMenuIM gheatingMenuIM = {
    .needs_redraw = true
};
