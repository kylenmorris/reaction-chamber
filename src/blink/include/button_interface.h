#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

#include <stdbool.h>

// Read raw button state; returns 1 if pressed
int hw_button_get_raw(int button_index);

#endif
