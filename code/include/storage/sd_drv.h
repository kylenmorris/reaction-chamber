#ifndef SD_INTERFACE_H
#define SD_INTERFACE_H

void save_to_file(char* filename, char* content);

char* load_file(char* filename);

void populate_file_list(const char* path);

void delete_file(char* filename);

#endif