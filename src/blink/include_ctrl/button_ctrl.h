#ifndef BUTTON_CTRL_H
#define BUTTON_CTRL_H

// Initializes the button pins
void button_ctrl_init(void);

// Checks for button presses with debouncing
// Writes to gButtonInput
void button_ctrl_step(void);

#endif