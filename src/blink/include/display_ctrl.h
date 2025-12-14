#ifndef DISPLAY_CTRL_H
#define DISPLAY_CTRL_H

#include "data_structs.h"

void terminal_draw_idle_menu();
void terminal_draw_heating_screen();
void draw_display(SystemState current_state);
void draw_progress_bar(int percentage);

#endif