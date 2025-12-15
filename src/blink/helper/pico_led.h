#ifndef PICO_LED_H
#define PICO_LED_H

#include <stdbool.h>

void pico_set_led(bool led_on);
int pico_led_init(void);

#endif