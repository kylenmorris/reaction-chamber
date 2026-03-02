#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "compilation_variables.h"
#include <stdint.h>

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
    #undef USE_HW_SD
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
#define NUM_BUTTONS 4
#define REDRAW_INTERVAL_MS 500

#define MAX_FILES 10
#define MAX_NAME_LEN 21 

// ####################################
// OPTICAL CONSTANTS
// ####################################

#define OPTICAL_REACTION_THRESHOLD 5000         // unsure of units

// ####################################
// BUTTON DEFINITIONS
// ####################################

#define BACK_BUTTON_GPIO_PIN 14     // btn 1
#define DOWN_BUTTON_GPIO_PIN 15     // btn 0  
#define UP_BUTTON_GPIO_PIN 12       // btn 3
#define SELECT_BUTTON_GPIO_PIN 13   // btn 2
// #define DEBUG_BUTTON_GPIO_PIN 

static const int BUTTON_PINS[] = {
    UP_BUTTON_GPIO_PIN,
    DOWN_BUTTON_GPIO_PIN,
    SELECT_BUTTON_GPIO_PIN,
    BACK_BUTTON_GPIO_PIN,
    // DEBUG_BUTTON_GPIO_PIN
};

// ####################################
// I2C DEFINITIONS
// ####################################

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5
#define I2C_BAUDRATE 100 * 1000  // 100 kHz

#define TEMP_SENSOR_0_I2C_ADDRESS 0x18
#define TEMP_SENSOR_1_I2C_ADDRESS 0x1C
#define TEMP_SENSOR_2_I2C_ADDRESS 0x1A
#define TEMP_SENSOR_3_I2C_ADDRESS 0x1E

// ####################################
// SPI DEFINITIONS
// ####################################

#define SPI0_SCK_PIN 18
#define SPI0_MOSI_PIN 19

#define SPI0_CSn_DISPLAY_PIN 17
#define DISPLAY_DC_A0_PIN 20    // A0 Data/Command pin - LOW=command, HIGH=data
#define DISPLAY_RST_PIN 21      // Reset pin - LOW=reset, HIGH=normal operation

#define SPI1_SCK_PIN 10
#define SPI1_MOSI_PIN 11
#define SPI1_MISO_PIN 8
#define SPI1_CSn_SD_CARD_PIN 0
// #define SPI1_BAUDRATE 125 * 1000 * 1000 / 16  // slowed down for testing, 7.8125 MHz 

#define SPI1_CS_SHIFT_SWITCHES_SR_PIN_0 26
#define SPI1_CS_SHIFT_SWITCHES_SR_PIN_1 27
// #define SPI1_CSn_ADC0_PIN 9
// #define SPI1_CSn_ADC1_PIN 7

// #define HEATER_ON_PIN 22

// #define HEATER_PIN 0

#define SPI_INSTANCE_DISPLAY spi0   // for reference only, see PICO2W.h
#define SPI_INSTANCE_SD spi1        // for reference only, see hw_config.h
#define SPI_INSTANCE_SW spi1        // for reference only

#define PROBE_PIN 1

#define SPI0_BAUDRATE 4 * 1000 * 1000 / 4       // 4 MHz which is a limit from glcd
#define SPI1_BAUDRATE 125 * 1000 * 1000 / 4     // 31.25 MHz, just the default from no-os-fatfs 


#endif