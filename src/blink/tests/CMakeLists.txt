cmake_minimum_required(VERSION 3.13)

add_executable(pathoscan_tests
    test_main.c
    ../system_state_loop.c
)

target_include_directories(pathoscan_tests PRIVATE
    ..
    ../model
)
