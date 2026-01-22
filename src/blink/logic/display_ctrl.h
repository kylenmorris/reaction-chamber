#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

// Initializes the display (terminal or LCD)
void display_ctrl_init(void);

// Draws the relevant display on either the terminal or LCD
// Reads from gSystemState and other structs
void display_ctrl_step(void);

#endif
