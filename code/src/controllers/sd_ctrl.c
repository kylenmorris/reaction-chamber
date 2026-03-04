#include "sd_ctrl.h"

#include "json_helper.h"
#include "data_structs.h"
#include "sd_drv.h"
#include <stdlib.h>

int get_latest_filename_int() {
    load_metadata_from_sd_card();
    return gSystemInfo.latest_filename_int;
}

void save_metadata_to_sd_card() {
    char *json_str = create_json_string();
    save_to_file("metadata.json", json_str);
    free(json_str);
}

void save_test_result_from_global_to_filename(char* filename) {
    char *test_results = serialize_test_results();
    save_to_file(filename, test_results);

    free(test_results);

    filenames_need_update = true;
}

void load_all_sd_filenames_into_global(void) {

    if (filenames_need_update == false) {
        return;
    }
    
    bool success = populate_file_list("");

    if (success) {
        filenames_need_update = false;
    } else {
        filenames_need_update = true; // error and try again next time
    }
    
}

void load_test_result_from_filename_into_global(char* filename) {
    
    char *test_string = load_file(filename);
    read_json_string(test_string);
    
    free(test_string);
}

void delete_file_from_sd_card(char* filename) {
    delete_file(filename);
}