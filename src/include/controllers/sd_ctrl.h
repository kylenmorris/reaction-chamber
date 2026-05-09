#ifndef SD_CTRL_H
#define SD_CTRL_H

// These are verbose and I like it that way. SD logic is a bit messy

void save_test_result_from_global_to_filename(char* filename);

void load_all_sd_filenames_into_global(void);

void load_test_result_from_filename_into_global(char* filename);

void delete_file_from_sd_card(char* filename);

int get_latest_filename_int(void);

#endif