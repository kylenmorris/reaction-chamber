#ifndef SYSTEM_STATE_LOOP_H
#define SYSTEM_STATE_LOOP_H

// The main logic loop for the system. Meant to be run periodically from main.c 
// Note does NOT include display or button handling, those are in core 1
void run_system_state_loop_core0(void);

void run_system_state_loop_core1(void);

void run_debug_loop(void);

#endif