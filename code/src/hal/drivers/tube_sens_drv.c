#include "drivers.h"

#ifdef USE_HW_TUBE_SENS

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

uint8_t hw_tube_sens_read_all(int pin) {
    uint8_t data;

    // Disable clock (CLKINH = 1)
    gpio_put(pin, 1);

    // Take MOSI away from SPI as I screwed up the pcb, plz forgive me
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SIO);
    gpio_set_dir(SPI1_MOSI_PIN, GPIO_OUT);

    // Parallel load
    gpio_put(SPI1_MOSI_PIN, 0);
    sleep_us(1);
    gpio_put(SPI1_MOSI_PIN, 1);
    sleep_us(1);

    // Give MOSI back to SPI
    gpio_set_function(SPI1_MOSI_PIN, GPIO_FUNC_SPI);

    // Enable clock
    gpio_put(pin, 0);

    // Clock 8 bits
    spi_read_blocking(SPI_INSTANCE_SW, 0xFF, &data, 1);

    // Disable clock again
    gpio_put(pin, 1);

    return ~data;   // because QH* is inverted output
}

#endif


#ifndef USE_HW_TUBE_SENS

// Track which tubes have been inserted via debug button
static bool used_tubes[NUM_TUBES] = { false };

uint8_t hw_tube_sens_read_all(void) {

    // Handle debug button press to insert new random tube
    if (handle_button_press(DEBUG)) {

        // printf("Debug button pressed, inserting tube\n");
        // Find an unused tube
        int available_tubes[NUM_TUBES];
        int available_count = 0;
        
        for (int i = 0; i < NUM_TUBES; i++) {
            if (!used_tubes[i]) {
                available_tubes[available_count++] = i;
            }
        }
        
        // If tubes are available, pick a random one
        if (available_count > 0) {
            int random_idx = random() % available_count;
            int tube_index = available_tubes[random_idx];

            // printf("Inserting tube %d\n", tube_index);
            
            gSysControl.tube_present[tube_index] = true;
            used_tubes[tube_index] = true;
        }
    }

    return 0;
    
}
#endif