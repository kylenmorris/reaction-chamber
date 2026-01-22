#include "button_interface.h"

#include "constants.h"
#include <hardware/gpio.h>

// internal function to init pins
static void init_gpio_as_button(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    gpio_pull_up(gpio_pin);
}

// ####################################
// PUBLIC METHODS
// ####################################

// Initialize required gpio pins as buttons
void hw_button_init(void) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        init_gpio_as_button(BUTTON_PINS[i]);
    }
}

// Checks for button presses and updates gButtonInput if a button was pressed
int hw_button_get_raw(int button_index) {
    return gpio_get(BUTTON_PINS[button_index]);
}