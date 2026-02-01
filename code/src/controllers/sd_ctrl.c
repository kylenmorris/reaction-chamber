#include "sd_ctrl.h"

#include "json_helper.h"
#include "sd_drv.h"
#include <stdlib.h>


void save_test_results_to_file(char* filename) {
    char *test_results = serialize_test_results();
    save_to_file(filename, test_results);

    free(test_results);
}


void load_all_filenames(void) {

}


void load_test_from_filename(char* filename) {
    char *test_string = load_file(filename);

    read_json_string(test_string);
    free(test_string);
}