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

    set(PICO_BOARD pico2 CACHE STRING "Board type")

    include(pico_sdk_import.cmake)

    project(main C CXX ASM)

    pico_sdk_init()

endif()

# ====================================================================================
# Model/Data Structs 
# ====================================================================================

add_library(model
    src/model/data_structs.c
    src/model/data_helpers.c
)
target_include_directories(model PUBLIC
    include/model
)

# Must be before any other compilation of this project
if (BUILD_TESTS)
    target_compile_definitions(model PUBLIC MASTER_SIMULATION_MODE)
endif()

# ====================================================================================
# no-OS-FatFS-SD 
# ====================================================================================

if (NOT BUILD_TESTS)
    add_subdirectory(lib/no-OS-FatFs-SD/src)
endif()

# ====================================================================================
# SD
# ====================================================================================

if (NOT BUILD_TESTS)
    add_library(sd_card
        src/storage/sd_drv.c    
        src/storage/hw_config.c
    )   
    target_include_directories(sd_card PUBLIC
        include/storage
        include/model
        lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src
        lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src/include    
    )   
    target_link_libraries(sd_card PUBLIC
        no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
        model        
    )

else()
    add_library(sd_card
        src/storage/sd_drv.c    
    )  
    target_include_directories(sd_card PUBLIC
        include/storage
        include/model
    )   
    target_link_libraries(sd_card PUBLIC
        model
    )
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

    target_include_directories(glcd PUBLIC 
        lib/glcd-master
        lib/glcd-master/devices
        lib/glcd-master/controllers
        include/model/
    )

    target_link_libraries(glcd PUBLIC 
        pico_stdlib 
        hardware_spi
    )

endif()

# ====================================================================================
# CJSON 
# ====================================================================================

add_library(cjson 
    lib/cjson/cjson.c
)
target_include_directories(cjson PUBLIC 
    lib/cjson/
)

# ====================================================================================
# DATA HELPER 
# ====================================================================================

add_library(data_helper 
    src/storage/json_helper.c
)
target_include_directories(data_helper PUBLIC 
    include/storage/
)
target_link_libraries(data_helper 
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
        sd_card
        pico_stdlib 
        hardware_spi
        hardware_i2c
        # pico_cyw43_arch_none
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
        sd_card
    )
else()
    target_link_libraries(logic_core PUBLIC
        drivers_sw
        sd_card
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
