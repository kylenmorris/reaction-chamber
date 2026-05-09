#ifndef COMPILATION_VARIABLES_H
#define COMPILATION_VARIABLES_H

// Prints extra info to the terminal
#define PRINT_DEBUG

// Disables all hardware, mostly intended for cmakelists to enable when running tests
// #define MASTER_SIMULATION_MODE

// Choose which components are hooked up to the pico
#define USE_HW_PICO
#define USE_HW_BUTTONS
#define USE_HW_HEATER
#define USE_HW_TEMP_SENS
#define USE_HW_OPTICAL
#define USE_HW_TUBE_SENS
#define USE_HW_DISPLAY
#define USE_HW_SD

#endif