# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(main
    main.c
    system_state_loop.c
    model/data_structs.c
    model/imodel_structs.c
    controller/button_ctrl.c
    controller/temp_sens_ctrl.c
    controller/tube_optical_ctrl.c
    controller/heater_ctrl.c
    controller/display_ctrl.c
    helper/pico_led.c

)

# Include paths
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/controller
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/model
    ${CMAKE_CURRENT_LIST_DIR}/helper
)

# pull in common dependencies
target_link_libraries(main pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(
        main 
        pico_cyw43_arch_none
        pico_multicore
        )
endif()

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)
