/**
   \file PICO2W.c
   \brief Functions relating to Raspberry Pi Pico 2W microcontroller.
          For use with Pico SDK

 */ 

#include "../glcd.h"
#include "fonts/font5x7.h"

#if defined(GLCD_DEVICE_PICO2W)

#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "../fonts/Liberation_Sans15x21_Numbers.h"

/**
 * \brief Initialize the GLCD for Pico 2W.
 * 
 * This function sets up:
 * - SPI interface for communication with the LCD controller
 * - GPIO pins for control signals (DC, CS, RST, A0)
 * - LCD controller specific initialization
 * 
 * The initialization sequence depends on the configured controller type.
 */
void glcd_init(void)
{
	/* Initialize GPIO for LCD control pins */
	gpio_init(PICO2W_DC_PIN);
	gpio_init(PICO2W_RST_PIN);
	gpio_init(PICO2W_A0_PIN);
	// gpio_init(PICO2W_CS_PIN);
	
	/* Set control pins as outputs */
	gpio_set_dir(PICO2W_DC_PIN, GPIO_OUT);
	gpio_set_dir(PICO2W_RST_PIN, GPIO_OUT);
	gpio_set_dir(PICO2W_A0_PIN, GPIO_OUT);
	// gpio_set_dir(PICO2W_CS_PIN, GPIO_OUT);
	
	/* Initialize control pins to idle states */
	// GLCD_DESELECT();   /* CS high */
	GLCD_DC_LOW();     /* Start in command mode */
	GLCD_RESET_HIGH(); /* Release reset */
	GLCD_A0_HIGH();    /* A0 high by default */
	
	// /* Initialize SPI interface */
	// spi_init(PICO2W_SPI_INSTANCE, PICO2W_SPI_BAUDRATE);
	
	// /* Set up SPI pins */
	// gpio_set_function(PICO2W_MOSI_PIN, GPIO_FUNC_SPI);
	// gpio_set_function(PICO2W_MISO_PIN, GPIO_FUNC_SPI);
	// gpio_set_function(PICO2W_SCK_PIN, GPIO_FUNC_SPI);
	
	// /* Small delay to allow SPI to initialize */
	// sleep_ms(1);

#if defined(GLCD_CONTROLLER_ST7565R)
	
	/* Controller-specific initialization for ST7565R */
	glcd_ST7565R_init();
	
	/* Initialize display mode */
	glcd_all_on();
	sleep_ms(500);
	glcd_normal();
	
	/* Set start line */
	glcd_set_start_line(0);
	
	/* Clear display immediately */
	glcd_clear_now();
	
	/* Select screen buffer */
	glcd_select_screen(glcd_buffer, &glcd_bbox);
	
	glcd_tiny_set_font(Font5x7,5,7,32,127);

	/* Clear the screen buffer */
	glcd_clear();

#else
	#error "LCD Controller not supported"
#endif /* GLCD_CONTROLLER_* */
	
}

/**
 * \brief Write a byte to the LCD controller via SPI.
 * 
 * This function transmits a single byte to the LCD controller using the
 * SPI bus. The DC (Data/Command) pin must be set appropriately before
 * calling this function.
 * 
 * \param c The byte to write to the LCD controller
 * \return The byte read from the SPI bus (usually not used for write-only operation)
 */
void glcd_spi_write(uint8_t c)
{
	GLCD_SELECT();
	spi_write_blocking(PICO2W_SPI_INSTANCE, &c, 1);
	GLCD_DESELECT();
}

/**
 * \brief Reset the LCD controller.
 * 
 * This function performs a hardware reset of the LCD controller by:
 * - Driving the RST pin low for a minimum pulse width
 * - Releasing the RST pin high to complete the reset
 * 
 * The reset pulse duration is defined by GLCD_RESET_TIME.
 */
void glcd_reset(void)
{
	/* Assert reset signal */
	GLCD_SELECT();
	GLCD_RESET_LOW();
	sleep_ms(GLCD_RESET_TIME);
	GLCD_RESET_HIGH();
	GLCD_DESELECT();
}

#endif /* defined(GLCD_DEVICE_PICO2W) */
