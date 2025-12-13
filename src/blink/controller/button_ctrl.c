#include "button_ctrl.h"
#include "constants.h"
#include "data_structs.h"
#include <hardware/gpio.h>

const int BUTTON_PINS[] = {
    UP_BUTTON_GPIO_PIN,
    DOWN_BUTTON_GPIO_PIN,
    SELECT_BUTTON_GPIO_PIN,
    BACK_BUTTON_GPIO_PIN
};

void button_init() {
    init_gpio_button(UP_BUTTON_GPIO_PIN);
    init_gpio_button(DOWN_BUTTON_GPIO_PIN);
    init_gpio_button(SELECT_BUTTON_GPIO_PIN);
    init_gpio_button(BACK_BUTTON_GPIO_PIN);
}


void button_step(void) {
    gButtonInput.wasPressed = false;

    for (int i = 0; i < NUM_BUTTONS; i++) {

        int reading = gpio_get(BUTTON_PINS[i]);

        // Check for stable input
        if (reading == gButtonInput.lastButtonStates[i]) {
            gButtonInput.debounceCycles[i]++;
        } 
        else { 
            // input is different, reset
            gButtonInput.debounceCycles[i] = 0;
        }

        // If stable long enough, update button state
        if (gButtonInput.debounceCycles[i] > DEBOUNCE_TIME_CYCLES) {

            if (reading != gButtonInput.buttonStates[i]) {
                gButtonInput.buttonStates[i] = reading;

                // Detect press
                if (reading == LOW) {
                    gButtonInput.lastPressed = BUTTON_PINS[i];
                    gButtonInput.wasPressed  = true;
                }
            }
        }

        gButtonInput.lastButtonStates[i] = reading;
    }
}

// internal function to initialize pins for button input
void init_gpio_button(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    gpio_pull_up(gpio_pin);
}
