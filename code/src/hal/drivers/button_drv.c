#include "drivers.h"

#ifdef USE_HW_BUTTONS

#include <hardware/gpio.h>

int hw_button_get_raw(int button_gpio) {
    return gpio_get(button_gpio);
}
#endif

#ifndef USE_HW_BUTTONS
int hw_button_get_raw(int button_gpio) {
    return 1; // no presses
}
#endif
