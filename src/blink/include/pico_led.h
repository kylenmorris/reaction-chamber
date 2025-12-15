#ifndef PICO_LED_H
#define PICO_LED_H

#include <stdbool.h>

// Initialize the pico onboard LED
int pico_led_init(void);

// Set the onboard LED state
void pico_set_led(bool led_on);

#endif