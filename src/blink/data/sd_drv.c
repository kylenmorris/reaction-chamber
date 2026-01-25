#include "sd_drv.h"

#include <stdio.h>

void save_to_file(char* string) {
    printf("Saving file...\n");
    FILE *f = fopen("../output/results.json", "w");
    if (f) {
        fputs(string, f);
        fclose(f);
    }
}