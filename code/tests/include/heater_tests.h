#ifndef HEATER_TESTS_H
#define HEATER_TESTS_H

void test_heater_not_active_during_temp_sensor_failure(void);
void test_heater_turns_on_when_heating(void);
void test_heater_shutoff_on_overheat(void);

void json_test(void);

#endif