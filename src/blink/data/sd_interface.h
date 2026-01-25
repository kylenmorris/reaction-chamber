#ifndef SD_INTERFACE_H
#define SD_INTERFACE_H

void save_to_sd(void);

void read_file_from_sd(void);

void read_all_files_from_sd(void);

char *create_results_json(void);

#endif