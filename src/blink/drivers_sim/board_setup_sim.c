#include "board_setup.h"

#include "constants.h"
#include <hardware/gpio.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <stdbool.h>

#include "glcd.h"
#include <hardware/gpio.h>
#include <stdbool.h>

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

int init_led(void) {

}

static void init_gpio_as_button(int gpio_pin) {

}

void board_setup(void) {

}