#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdbool.h> 

// Button types
typedef enum {
    UP,
    DOWN,
    SELECT,
    BACK
} ButtonType;

// Input state
typedef struct {
    ButtonType lastPressed;
    bool wasPressed;
} ButtonInput;

// Declare the global variable (no memory yet)
extern ButtonInput gButtonInput;

#endif
