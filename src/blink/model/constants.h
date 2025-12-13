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

#define UP_BUTTON_GPIO_PIN 19  
#define DOWN_BUTTON_GPIO_PIN 18  
#define SELECT_BUTTON_GPIO_PIN 20  
#define BACK_BUTTON_GPIO_PIN 16  


// ####################################
// REACTION CONSTANTS
// ####################################

#define NUM_TUBES 10

#define TEMP_HIGH_C 61
#define TEMP_HIGH_HIGH_C 64
#define TEMP_LOW_LOW_C 58

#define REACTION_DURATION_S 30 * 60
#define TEMP_LOW_DURATION_S 5 * 60
#define TEMP_EXTREME_DURATION_S 10


// ####################################
// DISPLAY AND INPUT CONSTANTS
// ####################################

#define DEBOUNCE_TIME_CYCLES 5 
#define NUM_BUTTONS 4




#endif