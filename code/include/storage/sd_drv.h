#ifndef SD_INTERFACE_H
#define SD_INTERFACE_H

void write_to_sd_card(void);

void save_to_file(char* filename, char* content);

char* load_file(char* filename);

#endif