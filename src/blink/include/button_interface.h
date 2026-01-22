
#ifndef BUTTON_CTRL_H
#define BUTTON_CTRL_H

#include "data_structs.h"
#include <stdbool.h>

//
void hw_button_init(void);

// 
int hw_button_get_raw(int button_index);

#endif
