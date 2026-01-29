#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "compilation_variables.h"

// ####################################
// SYSTEM DEFINITIONS
// ####################################

#define SYSTEM_DELAY_MS 10 // Main loop delay (10 ms = 100 Hz)

#define LOW 0
#define HIGH 1

#ifdef PRINT_DEBUG
    #define DEBUG_LOG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...) // Becomes nothing on the Pico, saving memory
#endif

#ifdef MASTER_SIMULATION_MODE
    #undef USE_HW_PICO
    #undef USE_HW_BUTTONS
    #undef USE_HW_HEATER
    #undef USE_HW_TEMP_SENS
    #undef USE_HW_OPTICAL
    #undef USE_HW_TUBE_SENS
    #undef USE_HW_DISPLAY
    #define IS_SIMULATING true
#else
    #define IS_SIMULATING false
#endif

// ####################################
// REACTION CONSTANTS
// ####################################

#define NUM_TUBES 10

#define TEMP_DESIRED_C 62.5
#define TEMP_LOW_C 61
#define TEMP_HIGH_HIGH_C 64
#define TEMP_LOW_LOW_C 58
#define LOWEST_POSSIBLE_TEMP -40 // for basic error checking on temp sensors
#define HIGHEST_POSSIBLE_TEMP 80

#define TEST_CYCLES 20

// #define REACTION_DURATION_MS 30 * 60 * 1000     // 30 minutes
#define REACTION_DURATION_MS 10 * 1000          // 10 seconds
#define TEMP_LOW_DURATION_MS 5 * 60 * 1000      // 5 minutes
#define TEMP_EXTREME_DURATION_MS 10 * 1000      // 10 seconds

// ####################################
// DISPLAY AND INPUT CONSTANTS
// ####################################

#define DEBOUNCE_TIME_CYCLES 5 
#define NUM_BUTTONS 5
#define REDRAW_INTERVAL_MS 500

// ####################################
// OPTICAL CONSTANTS
// ####################################

#define OPTICAL_REACTION_THRESHOLD 5000         // unsure of units

// ####################################
// PIN DEFINITIONS
// ####################################

#define BACK_BUTTON_GPIO_PIN 28 
#define DOWN_BUTTON_GPIO_PIN 27  
#define UP_BUTTON_GPIO_PIN 26 
#define SELECT_BUTTON_GPIO_PIN 22  
#define DEBUG_BUTTON_GPIO_PIN 15

static const int BUTTON_PINS[] = {
    UP_BUTTON_GPIO_PIN,
    DOWN_BUTTON_GPIO_PIN,
    SELECT_BUTTON_GPIO_PIN,
    BACK_BUTTON_GPIO_PIN,
    DEBUG_BUTTON_GPIO_PIN
};


#define DISPLAY_DC_A0_PIN 20   // A0 Data/Command pin - LOW=command, HIGH=data
#define DISPLAY_RST_PIN  21      // Reset pin - LOW=reset, HIGH=normal operation

// SPI STUFF

// #define SPI0_MISO_PIN 4
#define SPI0_SCK_PIN 18
#define SPI0_MOSI_PIN 19
#define SPI0_CS0_PIN 17 // display

#define HEATER_PIN 0

#define SPI1_SCK_PIN 1
#define SPI1_MISO_PIN 2
#define SPI1_MOSI_PIN 3

#define SPI1_CS0_PIN 4

#define SPI_INSTANCE_DISPLAY spi0       // note this doesn't update the SPI pin defines inside PICO2W.h
#define SPI_BAUDRATE_DISPLAY 4000000    // 4 MHz




#endif