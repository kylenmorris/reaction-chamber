#ifndef CONSTANTS_H
#define CONSTANTS_H

// ####################################
// SYSTEM DEFINITIONS
// ####################################

#define SYSTEM_DELAY_MS 10 // Main loop delay (10 ms = 100 Hz)

#define LOW 0
#define HIGH 1

// ####################################
// PIN DEFINITIONS
// ####################################

#define UP_BUTTON_GPIO_PIN 18  
#define DOWN_BUTTON_GPIO_PIN 16  
#define SELECT_BUTTON_GPIO_PIN 19  
#define BACK_BUTTON_GPIO_PIN 17 
#define DEBUG_BUTTON_GPIO_PIN 15

// ####################################
// REACTION CONSTANTS
// ####################################

#define NUM_TUBES 10

#define TEMP_LOW_C 61
#define TEMP_HIGH_HIGH_C 64
#define TEMP_LOW_LOW_C 58

#define REACTION_DURATION_MS 30 * 60 * 1000     // 30 minutes
#define TEMP_LOW_DURATION_MS 5 * 60 * 1000      // 5 minutes
#define TEMP_EXTREME_DURATION_MS 10 * 1000      // 10 seconds

// ####################################
// OPTICAL CONSTANTS
// ####################################

#define OPTICAL_REACTION_THRESHOLD 5000         // unsure of units

// ####################################
// DISPLAY AND INPUT CONSTANTS
// ####################################

#define DEBOUNCE_TIME_CYCLES 5 
#define NUM_BUTTONS 5
#define REDRAW_INTERVAL_MS 500




#endif