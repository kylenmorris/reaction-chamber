#include "json_helper.h"

#include "cJSON.h"
#include <stdio.h>

#include "data_structs.h"
#include "data_helpers.h"
#include "constants.h"
#include <string.h>
#include <stdlib.h>

// TODO check this as it's mostly from gemini

void read_metadata_json_string(char *string) {
    if (!string) return;

    cJSON *root = cJSON_Parse(string);
    if (!root) {
        printf("JSON Parse Error: %s\n", cJSON_GetErrorPtr());
        return;
    }

    cJSON *latest_filename_int = cJSON_GetObjectItemCaseSensitive(root, "latest_filename_int");
    if (latest_filename_int) {
        gSystemInfo.latest_filename_int = latest_filename_int->valueint;
    }

    cJSON_Delete(root);
}

void read_json_string(char *string) {
    if (!string) return;

    cJSON *root = cJSON_Parse(string);
    if (!root) {
        printf("JSON Parse Error: %s\n", cJSON_GetErrorPtr());
        return;
    }

    // 1. Parse Metadata
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "test_metadata");
    if (meta) {
        gTestStatus.reaction_total_time = cJSON_GetObjectItem(meta, "total_duration_sec")->valuedouble;
        gTestStatus.test_invalid = cJSON_IsTrue(cJSON_GetObjectItem(meta, "test_invalid"));
        
        cJSON *status = cJSON_GetObjectItem(meta, "final_gTestStatus");
        if (status && status->valuestring) {
            gTestStatus.completed = (strcmp(status->valuestring, "COMPLETED") == 0);
        }
    }

    // 2. Parse Results Array
    cJSON *results = cJSON_GetObjectItemCaseSensitive(root, "results");
    cJSON *tube_node = NULL;
    int i = 0;

    cJSON_ArrayForEach(tube_node, results) {
        if (i >= NUM_TUBES) break;

        // Extract values using helper macros or standard checks
        cJSON *id = cJSON_GetObjectItem(tube_node, "tube_id");
        cJSON *state_str = cJSON_GetObjectItem(tube_node, "state");
        cJSON *result_str = cJSON_GetObjectItem(tube_node, "result");
        cJSON *ctrl_str = cJSON_GetObjectItem(tube_node, "is_control");
        cJSON *ct = cJSON_GetObjectItem(tube_node, "detection_time_sec");
        cJSON *pos_det = cJSON_GetObjectItem(tube_node, "was_positive_detected");

        if (state_str) gTestStatus.tubes[i].state = string_to_state(state_str->valuestring);
        if (result_str) gTestStatus.tubes[i].result = string_to_result(result_str->valuestring);
        
        if (ctrl_str) {
            gTestStatus.tubes[i].is_positive_control = (strcmp(ctrl_str->valuestring, "POSITIVE_CONTROL") == 0);
            gTestStatus.tubes[i].is_negative_control = (strcmp(ctrl_str->valuestring, "NEGATIVE_CONTROL") == 0);
        }

        if (ct) gTestStatus.tubes[i].detection_time = ct->valuedouble;
        if (pos_det) gTestStatus.tubes[i].positive_detected = cJSON_IsTrue(pos_det);

        i++;
    }

    cJSON_Delete(root);
}

char* serialize_test_results() {
    cJSON *root = cJSON_CreateObject();

    // 1. Metadata Header
    cJSON *meta = cJSON_AddObjectToObject(root, "test_metadata");
    cJSON_AddNumberToObject(meta, "total_duration_sec", gTestStatus.reaction_total_time);
    cJSON_AddBoolToObject(meta, "test_invalid", gTestStatus.test_invalid);
    cJSON_AddStringToObject(meta, "final_gTestStatus", gTestStatus.completed ? "COMPLETED" : "INCOMPLETE");

    // 2. Results Array
    cJSON *results_array = cJSON_AddArrayToObject(root, "results");

    for (int i = 0; i < NUM_TUBES; i++) {
        cJSON *tube_obj = cJSON_CreateObject();
        
        cJSON_AddNumberToObject(tube_obj, "tube_id", i);
        cJSON_AddStringToObject(tube_obj, "state", get_tube_state_string(gTestStatus.tubes[i].state));
        cJSON_AddStringToObject(tube_obj, "result", get_result_string(gTestStatus.tubes[i].result));
        
        // Handle Control Type logic
        const char* ctrl = "NONE";
        if (gTestStatus.tubes[i].is_positive_control) ctrl = "POSITIVE_CONTROL";
        if (gTestStatus.tubes[i].is_negative_control) ctrl = "NEGATIVE_CONTROL";
        cJSON_AddStringToObject(tube_obj, "is_control", ctrl);

        cJSON_AddNumberToObject(tube_obj, "detection_time_sec", gTestStatus.tubes[i].detection_time);
        cJSON_AddBoolToObject(tube_obj, "was_positive_detected", gTestStatus.tubes[i].positive_detected);

        cJSON_AddItemToArray(results_array, tube_obj);
    }

    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    return out; // Remember to free() this later!
}
