#ifndef HW_CONFIG_H
#define HW_CONFIG_H

#include "constants.h"

#ifdef USE_HW_SD

    #include <stddef.h>

    #include "sd_card.h"

    /* Return the number of physical drives (SD card sockets) in the configuration */
    size_t sd_get_num();

    /* Return a pointer to the SD card "object" at the given physical drive number.
    (See http://elm-chan.org/fsw/ff/doc/filename.html#vol.)
    Parameter `num` must be less than sd_get_num(). */
    sd_card_t* sd_get_by_num(size_t num);

#endif

#endif