#ifndef DATA_HELPERS_H
#define DATA_HELPERS_H

#include "data_structs.h"

void reset_sim_params(void);

char* get_error_string(ErrorCode error);

ReactionResult string_to_result(const char* str);
TubeState string_to_state(const char* str);

char* get_system_state_string(SystemState state);
char* get_tube_state_string(TubeState state);
char* get_result_string(ReactionResult result);

bool handle_button_press(ButtonType button);

#endif