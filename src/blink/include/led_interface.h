#ifndef PICO_LED_H
#define PICO_LED_H

#include <stdbool.h>

int pico_led_init(void);

void pico_set_led(bool led_on);

#endif