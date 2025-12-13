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
        int truePress = debounceButton(i, reading);

        if (truePress) {
            gButtonInput.lastPressed = i;
            gButtonInput.wasPressed = true;
        }
    }
}

// internal function to init pins
void init_gpio_button(int gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    gpio_pull_up(gpio_pin);
}

// debounces a button input, returns 1 if a true press detected
int debounceButton(int i, int reading) {
    
    int truePress = 0;

    // Check for stable input
    if (reading == gButtonInput.lastButtonStates[i]) {
        gButtonInput.steadyTime[i]++;
    } 
    else { 
        // input is different, reset
        gButtonInput.steadyTime[i] = 0;
    }

    // If stable long enough, update button state
    if (gButtonInput.steadyTime[i] > DEBOUNCE_TIME_CYCLES) {

        // only if state changed is it a press
        if (reading != gButtonInput.buttonStates[i]) {
            gButtonInput.buttonStates[i] = reading;

            // press down, not release
            if (reading == LOW) {
                truePress = 1;
            }
        }
    }

    // save last reading
    gButtonInput.lastButtonStates[i] = reading;

    return truePress;
}
