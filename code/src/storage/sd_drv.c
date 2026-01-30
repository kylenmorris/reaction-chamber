#include "sd_drv.h"

#include <stdio.h>

#include "constants.h"

/**
 * @file main.c
 * @brief Minimal example of writing to a file on SD card
 * @details
 * This program demonstrates the following:
 * - Initialization of the stdio
 * - Mounting and unmounting the SD card
 * - Opening a file and writing to it
 * - Closing a file and unmounting the SD card
 */

#ifdef USE_HW_SD

#include "hw_config.h"
#include "f_util.h"
#include "ff.h"

void save_to_file(char* filename, char* content) {

    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html

    FATFS fs;
    FRESULT fr = f_mount(&fs, "", 1);
    if (FR_OK != fr) {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    // Open a file and write to it 
    FIL fil;
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr) {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }
    if (f_printf(&fil, content) < 0) {
        printf("f_printf to file failed!\n");
    }

    // Close the file
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    printf("Wrote to %s...", filename);
    
    // Unmount the SD card
    f_unmount("");
}

char *load_file(char *filename) {
    
    FATFS fs;
    FRESULT fr = f_mount(&fs, "", 1);
    if (FR_OK != fr) {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    // Open a file and read from it
    FIL fil;
    fr = f_open(&fil, filename, FA_READ);
    if (FR_OK != fr && FR_EXIST != fr) {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }

    UINT bytes_read;
    char raw_buffer[512];
    
    fr = f_read(&fil, raw_buffer, sizeof(raw_buffer) - 1, &bytes_read);
    if (fr == FR_OK) {
        raw_buffer[bytes_read] = '\0'; // Null-terminate for printing
        printf("Read %u bytes: %s\n", bytes_read, raw_buffer);
    }

    // Close the file
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    // Unmount the SD card
    f_unmount("");

    // return &raw_buffer;
}

#endif


#ifndef USE_HW_SD

void save_to_file(char* filename, char* content) {

}

char *load_file(char *filename) {

}

#endif