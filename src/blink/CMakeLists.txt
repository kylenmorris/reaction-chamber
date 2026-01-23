# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# Set controller directory based on option
if(USE_TEST_CONTROLLERS)
    set(CONTROLLER_DIR test_controllers)
else()
    set(CONTROLLER_DIR ../logic)
endif()

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
# ====================================================================================

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

# ====================================================================================
# --- GLCD Driver Library ---
# ====================================================================================

add_library(glcd STATIC
    lib/glcd-master/glcd.c
    lib/glcd-master/graphics.c
    lib/glcd-master/graphs.c
    lib/glcd-master/text.c
    lib/glcd-master/text_tiny.c
    lib/glcd-master/devices/PICO2W.c
    lib/glcd-master/controllers/ST7565R.c
)

# PUBLIC tells CMake: "If you link to GLCD, you also need these include paths"
target_include_directories(glcd PUBLIC 
    lib/glcd-master
    lib/glcd-master/devices
    lib/glcd-master/controllers
    model/
)

# Link the Pico SDK to the driver
target_link_libraries(glcd PUBLIC 
    pico_stdlib 
    hardware_spi
)

# ====================================================================================
# --- Application Logic & Controllers ---
# ====================================================================================

add_library(project_core STATIC
    system_state_loop.c
    test_manager.c

    model/data_structs.c
    model/imodel_structs.c

    logic/button_ctrl.c
    logic/temp_sens_ctrl.c
    logic/tube_optical_ctrl.c
    logic/heater_ctrl.c
    logic/tube_sens_ctrl.c
    logic/display_ctrl.c

    drivers/board_setup.c
    drivers/drivers.c
    drivers/screens_hw.c
    drivers/screens_sim.c
)

target_include_directories(project_core PUBLIC 
    logic
    model
    include
    include_ctrl
    drivers
    drivers_sim
)

target_link_libraries(project_core PUBLIC 
    glcd
    pico_stdlib 
    hardware_spi
    pico_cyw43_arch_none
)

# ====================================================================================
# Add executable
# ====================================================================================

add_executable(main
    main.c
)

# Include paths
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include_ctrl
    ${CMAKE_CURRENT_LIST_DIR}/model
    ${CMAKE_CURRENT_LIST_DIR}/drivers
    ${CMAKE_CURRENT_LIST_DIR}/drivers_sim
    ${CMAKE_CURRENT_LIST_DIR}/display
    ${CMAKE_CURRENT_LIST_DIR}/lib/glcd-master
)

# pull in common dependencies
target_link_libraries(main 
    glcd
    project_core
    pico_stdlib 
    hardware_spi
)

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
