#include "button_ctrl.h"

#include "drivers.h"

#include "constants.h"
#include "data_structs.h"

// debounces a button input, returns 1 if a true press detected
static int debounceButton(int i, int reading) {
    
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

// ####################################
// PUBLIC METHODS
// ####################################

// Checks for button presses and updates gButtonInput if a button was pressed
void button_ctrl_step(void) {

    gButtonInput.wasPressed = false;

    for (int i = 0; i < NUM_BUTTONS; i++) {

        int reading = hw_button_get_raw(BUTTON_PINS[i]);
        int truePress = debounceButton(i, reading);

        if (truePress) {
            gButtonInput.lastPressed = i;
            gButtonInput.wasPressed = true;
        }
    }
    
}

