cmake_minimum_required(VERSION 3.13)

project(system_tests C)

# Add definitions so the code knows it's in sim mode
add_compile_definitions(MASTER_SIMULATION_MODE)

# Include directories (point to your main source folders)
include_directories(
    ../include
    ../model
    ../logic
)

# Create the test executable
add_executable(run_tests 
    test_main.c
    ../logic/system_state_loop.c
    ../logic/heater_ctrl.c      # Add all your logic files here
    ../logic/temp_sens_ctrl.c
    ../drivers/drivers_master.c # This is your unified driver file
)

# If you use math.h
target_link_libraries(run_tests m)