#include "json_helper.h"

#include "cJSON.h"
#include <stdio.h>

#include "data_structs.h"
#include "constants.h"

void read_json_string(char *string) {

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

        cJSON_AddNumberToObject(tube_obj, "ct_time_sec", gTestStatus.tubes[i].ct_time);
        cJSON_AddBoolToObject(tube_obj, "was_positive_detected", gTestStatus.tubes[i].positive_detected);

        cJSON_AddItemToArray(results_array, tube_obj);
    }

    char *out = cJSON_Print(root);
    cJSON_Delete(root);
    return out; // Remember to free() this later!
}

//NOTE: Returns a heap allocated string, you are required to free it after use.
char *create_json_string(void)
{
    const unsigned int resolution_numbers[3][2] = {
        {1280, 720},
        {1920, 1080},
        {3840, 2160}
    };
    char *string = NULL;
    cJSON *resolutions = NULL;
    size_t index = 0;

    cJSON *monitor = cJSON_CreateObject();

    if (cJSON_AddStringToObject(monitor, "name", "Awesome 4K") == NULL)
    {
        goto end;
    }

    resolutions = cJSON_AddArrayToObject(monitor, "resolutions");
    if (resolutions == NULL)
    {
        goto end;
    }

    for (index = 0; index < (sizeof(resolution_numbers) / (2 * sizeof(int))); ++index)
    {
        cJSON *resolution = cJSON_CreateObject();

        if (cJSON_AddNumberToObject(resolution, "width", resolution_numbers[index][0]) == NULL)
        {
            goto end;
        }

        if (cJSON_AddNumberToObject(resolution, "height", resolution_numbers[index][1]) == NULL)
        {
            goto end;
        }

        cJSON_AddItemToArray(resolutions, resolution);
    }

    string = cJSON_Print(monitor);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print monitor.\n");
    }


end:
    cJSON_Delete(monitor);
    return string;
}

