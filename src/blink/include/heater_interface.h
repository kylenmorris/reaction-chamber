#ifndef HEATER_CTRL_H
#define HEATER_CTRL_H

#include <stdbool.h>

// Any startup needed... might be empty
void heater_init(void);

// 
void heater_toggle(bool enable);

#endif