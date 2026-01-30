/**
 * \file PICO2W.h
 * \brief Pinouts for Raspberry Pi Pico 2W microcontroller.
 */

#ifndef GLCD_PINOUTS_PICO2W_H_
#define GLCD_PINOUTS_PICO2W_H_

#include "pico/stdlib.h"
#include "constants.h"

/**
 * \name SPI Configuration
 * @{
 */
#define PICO2W_SPI_INSTANCE SPI_INSTANCE_DISPLAY  /**< SPI instance to use (spi0 or spi1) */
#define PICO2W_SPI_BAUDRATE SPI0_BAUDRATE  /**< SPI clock rate in Hz (max 4MHz for LCD) */

/**
 * \name SPI Pin Definitions
 * These pins define the SPI bus connections to the LCD controller
 * @{
 */
#define PICO2W_MOSI_PIN SPI0_MOSI_PIN   /**< GPIO pin for SPI MOSI (Master Out Slave In) */
#define PICO2W_MISO_PIN SPI0_MISO_PIN   /**< GPIO pin for SPI MISO (Master In Slave Out) */
#define PICO2W_SCK_PIN  SPI0_SCK_PIN    /**< GPIO pin for SPI Clock (SCK) */
/**@}*/

/**
 * \name LCD Control Pin Definitions
 * These pins control the LCD controller (data, chip select, reset)
 * @{
 */
#define PICO2W_DC_PIN   DISPLAY_DC_A0_PIN   /**< Data/Command pin - LOW=command, HIGH=data */
#define PICO2W_CS_PIN   SPI0_CSn_DISPLAY_PIN   /**< Chip Select pin - LOW=selected, HIGH=deselected */
#define PICO2W_RST_PIN  DISPLAY_RST_PIN   /**< Reset pin - LOW=reset, HIGH=normal operation */
#define PICO2W_A0_PIN   DISPLAY_DC_A0_PIN   /**< Address 0 pin (used by some controllers like ST7565R) */
/**@}*/

/**
 * \name Timing Constants
 * @{
 */
#define GLCD_RESET_TIME 5   /**< Reset pulse duration in milliseconds */
/**@}*/

/**
 * \name LCD Control Macros
 * These macros provide a convenient interface for controlling LCD signals
 * @{
 */
#define GLCD_SELECT()     gpio_put(PICO2W_CS_PIN, 0)    /**< Select LCD (CS low) */
#define GLCD_DESELECT()   gpio_put(PICO2W_CS_PIN, 1)    /**< Deselect LCD (CS high) */
#define GLCD_DC_LOW()     gpio_put(PICO2W_DC_PIN, 0)    /**< Command mode (DC low) */
#define GLCD_DC_HIGH()    gpio_put(PICO2W_DC_PIN, 1)    /**< Data mode (DC high) */
#define GLCD_RESET_LOW()  gpio_put(PICO2W_RST_PIN, 0)   /**< Assert reset (RST low) */
#define GLCD_RESET_HIGH() gpio_put(PICO2W_RST_PIN, 1)   /**< Release reset (RST high) */
#define GLCD_A0_LOW()     gpio_put(PICO2W_A0_PIN, 0)    /**< A0 low */
#define GLCD_A0_HIGH()    gpio_put(PICO2W_A0_PIN, 1)    /**< A0 high */
/**@}*/

#endif /* defined(GLCD_DEVICE_PICO2W) */
