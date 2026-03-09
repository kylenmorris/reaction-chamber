#include "sd_drv.h"

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "data_structs.h"
#include "json_helper.h"

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
        FILE *fptr;
        long file_size;
        char *buffer;

        // Open the file in reading mode
        fptr = fopen(filename, "r");
        if (fptr == NULL) {
            printf("Error opening file: %s\n", filename);
            return NULL;
        }

        // Move the file pointer to the end of the file to get its size
        fseek(fptr, 0, SEEK_END);
        file_size = ftell(fptr);
        rewind(fptr); // Move the file pointer back to the beginning
        // Allocate memory to hold the file contents
        buffer = (char *)malloc(file_size + 1); // +1 for null termin
        if (buffer == NULL) {
            printf("Memory allocation failed\n");
            fclose(fptr);
            return NULL;
        }
        // Read the file contents into the buffer
        fread(buffer, sizeof(char), file_size, fptr);
        buffer[file_size] = '\0'; //
        // Close the file
        fclose(fptr);
        return buffer; // Caller is responsible for freeing this memory
    }

    void populate_file_list(const char* path) {
        // For simulation, we'll just populate with some dummy filenames
        snprintf(results_menu_items[0], MAX_NAME_LEN, "test_result_1.json");
        snprintf(results_menu_items[1], MAX_NAME_LEN, "test_result_2.json");
        snprintf(results_menu_items[2], MAX_NAME_LEN, "test_result_3.json");
        snprintf(results_menu_items[3], MAX_NAME_LEN, "test_result_4.json");

        gHistoryIM.num_items = 4; // Update the count of items
    }

    void delete_file(char* filename) {
        printf("Simulated deletion of file: %s\n", filename);
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
            return;
        }

        FIL fil;
        fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
        if (FR_OK != fr && FR_EXIST != fr) {
            printf("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
            f_unmount("");
            return;
        }
        if (f_printf(&fil, content) < 0) {
            printf("f_printf to file failed!\n");
        }

        // Close the file
        fr = f_close(&fil);
        if (FR_OK != fr) {
            printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
            f_unmount("");
            return;
        }

        printf("File data written to %s successfully.\n", filename);
        
        // Unmount the SD card
        f_unmount("");
    }

    static char raw_buffer[2048];

    char *load_file(char *filename) {

        FATFS fs;
        FRESULT fr = f_mount(&fs, "", 1); 
        if (fr != FR_OK) {
            printf("Mount failed: %d\n", fr);
            return NULL;
        }

        // Open a file and read from it
        FIL fil;
        fr = f_open(&fil, filename, FA_READ);
        if (FR_OK != fr && FR_EXIST != fr) {
            printf("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
            f_unmount("");
            return NULL;
        }

        UINT bytes_read;
        
        fr = f_read(&fil, raw_buffer, sizeof(raw_buffer) - 1, &bytes_read);
        if (fr == FR_OK) {
            raw_buffer[bytes_read] = '\0'; // Null-terminate for printing
            // printf("Read %u bytes: %s\n", bytes_read, raw_buffer);
            printf("Read %u bytes from file %s successfully.\n", bytes_read, filename);
        }

        // Close the file
        fr = f_close(&fil);
        if (FR_OK != fr) {
            printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);

            f_unmount("");
            return NULL;
        }

        // Unmount the SD card
        f_unmount("");

        printf("File %s loaded successfully.\n", filename);

        return strdup(raw_buffer); // Caller is responsible for freeing this memory
    }

    bool populate_file_list(const char* path) {
        FRESULT res;
        DIR dir;
        static FILINFO fno;
        FATFS fs;

        int file_count = 0;

        res = f_mount(&fs, "", 1); 
        if (res != FR_OK) {
            printf("Mount failed: %d\n", res);
            gSystemError.current_error = ERROR_SD_READ_FAILED;
            return false;
        }

        // 2. Open the path passed into the function (e.g., "/")
        res = f_opendir(&dir, path); 
        if (res != FR_OK) {
            printf("Failed to open directory: %s (Error: %d)\n", path, res);
            f_unmount(""); // Clean up
            return false;
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

        gHistoryIM.num_items = file_count;

        return true;
    }

    void delete_file(char* filename) {
        FRESULT res;
        FATFS fs;

        res = f_mount(&fs, "", 1); 
        if (res != FR_OK) {
            printf("Mount failed: %d\n", res);
            return;
        }

        res = f_unlink(filename);
        if (res != FR_OK) {
            printf("Failed to delete file: %s (Error: %d)\n", filename, res);
        } else {
            printf("File deleted successfully: %s\n", filename);
        }

        f_unmount("");
    }

    void load_metadata_from_sd_card(void) {
        char *metadata_str = load_file("metadata.json");
        if (metadata_str) {
            read_metadata_json_string(metadata_str);
            free(metadata_str);
        } else {
            printf("Failed to load metadata.json\n");
        }
    }

#endif

