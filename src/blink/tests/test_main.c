#include <stdio.h>
#include <assert.h>
#include "data_structs.h"
#include "imodel_structs.h"
#include "system_state_loop.h"

void test_idle_to_heating(void);

int main(void) {
    test_idle_to_heating();

    printf("ALL TESTS PASSED\n");
    return 0;
}

void test_idle_to_heating(void) {

    gSystemState = IDLE;
    gButtonInput.wasPressed = true;
    gButtonInput.lastPressed = SELECT;
    gIdleMenuIM.selected_index = 0; // should paramterize this somehow so its always 'start test'

    run_system_state_loop();

    assert(gSystemState == HEATING);
}