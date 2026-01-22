#include "led_interface.h"

#include <stdbool.h>
#include <stdio.h>

int pico_led_init(void) {
    return 0;
}

// Turn the led on or off
void pico_set_led(bool led_on) {
    printf("LED is %s\n", led_on ? "ON" : "OFF");
}
