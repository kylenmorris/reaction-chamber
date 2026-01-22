#include "button_interface.h"

#include <hardware/gpio.h>
#include "constants.h"

// Checks for button presses and updates gButtonInput if a button was pressed
int hw_button_get_raw(int button_index) {
    return gpio_get(BUTTON_PINS[button_index]);
}