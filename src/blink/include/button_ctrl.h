
#ifndef BUTTON_CTRL_H
#define BUTTON_CTRL_H

#include "data_structs.h"
#include <stdbool.h>

//
void button_ctrl_init(void);

// 
void button_ctrl_step(void);

// Check if a specific button was pressed
bool button_ctrl_is_pressed(ButtonType button);

#endif
