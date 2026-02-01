#include "sd_ctrl.h"

#include "json_helper.h"
#include "sd_drv.h"
#include <stdlib.h>

void save_test_result_from_global_to_filename(char* filename) {
    char *test_results = serialize_test_results();
    save_to_file(filename, test_results);

    free(test_results);
}

void load_all_sd_filenames_into_global(void) {
    populate_file_list("");
}

void load_test_result_from_filename_into_global(char* filename) {
    char *test_string = load_file(filename);
    read_json_string(test_string);
    
    free(test_string);
}