#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define EXPECT_ONCE(condition, format, ...) \
    if (!(condition)) { \
        printf("\033[1;31m[FAIL]\033[0m %s (Line %d): \n", __FUNCTION__, __LINE__); \
        printf(format, ##__VA_ARGS__); \
        printf("\n"); \
    } else { \
    } \

#define EXPECT(condition, format, ...) \
    if (!(condition)) { \
        printf("\033[1;31m[FAIL]\033[0m %s (Line %d): \n", __FUNCTION__, __LINE__); \
        printf(format, ##__VA_ARGS__); \
        printf("\n"); \
        break; \
    } else { \
    } \

#endif
