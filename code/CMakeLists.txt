# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS OFF CACHE BOOL "Build simulation tests")
endif()

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
# ====================================================================================

if (BUILD_TESTS)
    # This prevents the Pico VS Code Extension from forcing ARM toolchains
    set(PICO_SDK_FETCH_FROM_GIT OFF)
    set(SKIP_PICO_SDK_IMPORT ON) 
    
    # Define a standard Windows project immediately
    project(main C CXX)

else()

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

    set(PICO_BOARD pico2_w CACHE STRING "Board type")

    include(pico_sdk_import.cmake)

    project(main C CXX ASM)

    pico_sdk_init()

endif()

# ====================================================================================
# no-OS-FatFS-SD 
# ====================================================================================

if (NOT BUILD_TESTS)
    add_subdirectory(lib/no-OS-FatFs-SD/src)
endif()

# ====================================================================================
# GLCD 
# ====================================================================================

if (NOT BUILD_TESTS)

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
        include/model/
    )

    # Link the Pico SDK to the driver
    target_link_libraries(glcd PUBLIC 
        pico_stdlib 
        hardware_spi
    )

endif()

# ====================================================================================
# Model/Data Structs 
# ====================================================================================

add_library(model
    src/model/data_structs.c
    src/model/imodel_structs.c
)
target_include_directories(model PUBLIC
    include/model
)

# Enables all software driver definitions, disables all hardware definitions. See compilation_variables.h
if (BUILD_TESTS)
    target_compile_definitions(model PUBLIC MASTER_SIMULATION_MODE)
endif()

# ====================================================================================
# CJSON 
# ====================================================================================

add_library(cjson lib/cjson/cjson.c)
target_include_directories(cjson PUBLIC lib/cjson/)

add_library(data_helper 
    src/storage/json_helper.c
    src/storage/sd_drv.c    
    src/storage/hw_config.c
)
target_include_directories(data_helper PUBLIC 
    include/storage/
    lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src
    lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src/include
)
target_link_libraries(data_helper 
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico    
    cjson
    model
)

# ====================================================================================
# Hardware drivers
# ====================================================================================

if (NOT BUILD_TESTS)
    add_library(drivers_hw
        src/hal/hal_hw/board_setup.c
        src/hal/hal_hw/drivers_hw.c
        src/hal/hal_hw/screens_hw.c
    )
    target_include_directories(drivers_hw PUBLIC 
        include/interfaces
    )
    target_link_libraries(drivers_hw PUBLIC 
        glcd
        pico_stdlib 
        hardware_spi
        pico_cyw43_arch_none
        model
        cjson
        data_helper
    )
endif()

# ====================================================================================
# Simulated drivers
# ====================================================================================

add_library(drivers_sw
    src/hal/hal_sw/drivers_sw.c
    src/hal/hal_sw/screens_sw.c
)
target_include_directories(drivers_sw PUBLIC 
    include/interfaces
)
target_link_libraries(drivers_sw PUBLIC 
    model
    cjson
    data_helper
)

# ====================================================================================
# Logic core of program
# ====================================================================================

add_library(logic_core
    src/core/system_state_loop.c
    src/core/test_manager.c

    src/controllers/button_ctrl.c
    src/controllers/temp_sens_ctrl.c
    src/controllers/tube_optical_ctrl.c
    src/controllers/heater_ctrl.c
    src/controllers/tube_sens_ctrl.c
    src/controllers/display_ctrl.c
    src/controllers/sd_ctrl.c
)
target_include_directories(logic_core PUBLIC
    include/controllers
    include/core
)

if (NOT BUILD_TESTS)
    target_link_libraries(logic_core PUBLIC 
        drivers_hw # must be first 
        drivers_sw
    )
else()
    target_link_libraries(logic_core PUBLIC
        drivers_sw
    )

endif()

# ====================================================================================
# Tests library 
# ====================================================================================

if (BUILD_TESTS)
    add_library(tests_core 
        tests/test_files/heater_tests.c
        tests/test_files/state_tests.c
    )
    target_include_directories(tests_core PUBLIC
        tests/include
    )
    target_link_libraries(tests_core PUBLIC
        logic_core
    )
endif()

# ====================================================================================
# Executable 
# ====================================================================================

if (NOT BUILD_TESTS)
    add_executable(main
        src/main.c
    )

    target_link_libraries(main 
        logic_core
        pico_stdlib
        pico_multicore
    )
    
    pico_enable_stdio_usb(main 1)
    pico_enable_stdio_uart(main 0)
    pico_add_extra_outputs(main)

else()
    add_executable(test_main
        tests/test_main.c
    )

    target_link_libraries(test_main 
        tests_core
    )

endif()
