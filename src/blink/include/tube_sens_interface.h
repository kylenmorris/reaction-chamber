#ifndef TUBE_SENS_INTERFACE_H
#define TUBE_SENS_INTERFACE_H

#include <stdint.h>

// Returns a bitmask where bit 0 is Tube 1, bit 1 is Tube 2, etc.
uint16_t hw_tube_sens_read_all(void);

#endif