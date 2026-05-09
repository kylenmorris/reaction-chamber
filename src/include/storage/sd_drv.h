#ifndef SD_INTERFACE_H
#define SD_INTERFACE_H

#include <stdbool.h>

void save_to_file(char* filename, char* content);

char* load_file(char* filename);

bool populate_file_list(const char* path);

void delete_file(char* filename);

void load_metadata_from_sd_card(void);

#endif