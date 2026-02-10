#ifndef TEST_MANAGER_H
#define TEST_MANAGER_H

#include <stdbool.h>

void reset_test_data(void);
bool check_conditions_for_test_start(void);
bool check_test_completed(void);
void determine_results(void);
void test_step(void);

#endif