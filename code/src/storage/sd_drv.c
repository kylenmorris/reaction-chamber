#include "sd_drv.h"

#include <stdio.h>
#include "constants.h"
#include "data_structs.h"

#ifndef USE_HW_SD

    void save_to_file(char* filename, char* content) {
         FILE *fptr;

        // Open a file in writing mode
        fptr = fopen(filename, "w");

        // Write some text to the file
        fprintf(fptr, "%s", content);

        // Close the file
        fclose(fptr); 
    }

    char *load_file(char *filename) {

    }

#endif

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
        
        FATFS fs;
        FRESULT fr = f_mount(&fs, "", 1); 
        if (fr != FR_OK) {
            printf("Mount failed: %d\n", fr);
        }

        FIL fil;
        fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
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
        
        // Unmount the SD card
        f_unmount("");
    }

    char *load_file(char *filename) {

        FATFS fs;
        FRESULT fr = f_mount(&fs, "", 1); 
        if (fr != FR_OK) {
            printf("Mount failed: %d\n", fr);
        }

        // Open a file and read from it
        FIL fil;
        fr = f_open(&fil, filename, FA_READ);
        if (FR_OK != fr && FR_EXIST != fr) {
            panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
        }

        UINT bytes_read;
        char raw_buffer[2048];
        
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

    }

    void populate_file_list(const char* path) {
        FRESULT res;
        DIR dir;
        static FILINFO fno;
        FATFS fs;

        int file_count = 0;

        res = f_mount(&fs, "", 1); 
        if (res != FR_OK) {
            printf("Mount failed: %d\n", res);
            return;
        }

        // 2. Open the path passed into the function (e.g., "/")
        res = f_opendir(&dir, path); 
        if (res != FR_OK) {
            printf("Failed to open directory: %s (Error: %d)\n", path, res);
            f_unmount(""); // Clean up
            return;
        }

        memset(results_menu_items, 0, sizeof(results_menu_items));
        
        while (file_count < MAX_FILES) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;

            // Skip hidden files and the .sysinfo.txt file
            if (!(fno.fattrib & (AM_DIR | AM_HID)) && (fno.fname[0] != '.')) {
                // Copy name to array and null-terminate safely
                strncpy(results_menu_items[file_count], fno.fname, MAX_NAME_LEN - 1);
                results_menu_items[file_count][MAX_NAME_LEN - 1] = '\0';
                file_count++;
            }
        }

        f_closedir(&dir);

    }

#endif

