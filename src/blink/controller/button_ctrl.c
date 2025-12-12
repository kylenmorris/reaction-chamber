#include "../include/data_structs.h"
#include <hardware/gpio.h>

void button_step() {
    gButtonInput.wasPressed = !gpio_get(15);
}
