cmake_minimum_required(VERSION 3.13)

# Set controller directory based on option
if(USE_TEST_CONTROLLERS)
    set(CONTROLLER_DIR test_controllers)
else()
    set(CONTROLLER_DIR ../logic)
endif()

add_executable(pathoscan_tests
    test_main.c
    ${CONTROLLER_DIR}/system_state_loop.c
    ${CONTROLLER_DIR}/test_manager.c
    ../model/data_structs.c
    ../model/imodel_structs.c
    ${CONTROLLER_DIR}/button_ctrl.c
    ${CONTROLLER_DIR}/temp_sens_ctrl.c
    ${CONTROLLER_DIR}/tube_optical_ctrl.c
    ${CONTROLLER_DIR}/heater_ctrl.c
    ${CONTROLLER_DIR}/tube_sens_ctrl.c
)

target_include_directories(pathoscan_tests PRIVATE
    ..
    ../logic
    ../include
    ../include_ctrl
    ../model
    ../drivers
    ../drivers_sim
)
