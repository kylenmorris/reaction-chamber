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

#define UP_BUTTON_GPIO_PIN 16  
#define DOWN_BUTTON_GPIO_PIN 18  
#define SELECT_BUTTON_GPIO_PIN 17  
#define BACK_BUTTON_GPIO_PIN 19 
#define DEBUG_BUTTON_GPIO_PIN 15

static const int BUTTON_PINS[] = {
    UP_BUTTON_GPIO_PIN,
    DOWN_BUTTON_GPIO_PIN,
    SELECT_BUTTON_GPIO_PIN,
    BACK_BUTTON_GPIO_PIN,
    DEBUG_BUTTON_GPIO_PIN
};


// ####################################
// REACTION CONSTANTS
// ####################################

#define NUM_TUBES 10

#define TEMP_LOW_C 61
#define TEMP_HIGH_HIGH_C 64
#define TEMP_LOW_LOW_C 58

// #define REACTION_DURATION_MS 30 * 60 * 1000     // 30 minutes
#define REACTION_DURATION_MS 10 * 1000          // 10 seconds
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

#define DISPLAY_DC_A0_PIN   7   // A0 Data/Command pin - LOW=command, HIGH=data
#define DISPLAY_RST_PIN  6      // Reset pin - LOW=reset, HIGH=normal operation

// ####################################
// SPI CONSTANTS
// ####################################

#define SPI0_SCK_PIN 2
#define SPI0_MISO_PIN 4
#define SPI0_MOSI_PIN 3

#define SPI0_CS0_PIN 5 // display

#define SPI1_SCK_PIN 14
#define SPI1_MISO_PIN 12
#define SPI1_MOSI_PIN 11

#define SPI1_CS0_PIN 13 // SD card reader

// Check glcd/devices/PICO2W.h for usage

#define SPI_INSTANCE_DISPLAY spi0       // spi0 or spi1 - note this doesn't update the SPI pin defines inside PICO2W.h
#define SPI_BAUDRATE_DISPLAY 4000000    // 4 MHz




#endif