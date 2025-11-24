cmake_minimum_required(VERSION 3.22)

#
# USB-CAN Converter Project for AT32F403A/407
#

# Core project settings
project(USB-CAN-Converter)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Select MCU
set(MCU_TYPE "AT32F407VGT7")
set(TAFCO_MCU_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Optimization settings
if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    message(STATUS "RELEASE mode with -O2")
    add_compile_options(-O2)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    message(STATUS "DEBUG mode with -Og -g3")
    add_compile_options(-Og -g3)
endif()

# CPU parameters
set(cpu_PARAMS
    -mthumb
    -mcpu=cortex-m4
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
)

# AT32F403A/407 HAL Drivers
set(AT32_HAL_PATH ${TAFCO_MCU_CORE_PATH}/AT32F403A)

# Source files
set(sources_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_can_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/at32f403a_407_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/at32f403a_407_int.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/at32f403a_407_board.c
    
    # AT32F403A HAL drivers
    ${AT32_HAL_PATH}/cmsis/cm4/device_support/system_at32f403a_407.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_crm.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_gpio.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_misc.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_usart.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_can.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_usb.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_acc.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_flash.c
    ${AT32_HAL_PATH}/drivers/src/at32f403a_407_tmr.c
    
    # Startup file
    ${AT32_HAL_PATH}/startup/startup_at32f403a_407.s
)

# Include directories
set(include_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${AT32_HAL_PATH}/cmsis/cm4/core_support
    ${AT32_HAL_PATH}/cmsis/cm4/device_support
    ${AT32_HAL_PATH}/drivers/inc
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_drivers/inc
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_class/cdc
)

# USB Middleware sources
set(USB_SOURCES
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_drivers/src/usbd_core.c
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_drivers/src/usbd_int.c
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_drivers/src/usbd_sdr.c
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_class/cdc/cdc_class.c
    ${TAFCO_MCU_CORE_PATH}/Middlewares/USB/usbd_class/cdc/cdc_desc.c
)

list(APPEND sources_SRCS ${USB_SOURCES})

# Symbols definition
set(symbols_SYMB
    AT32F407VGT7
    USE_STDPERIPH_DRIVER
)

# Linker script
if("${MCU_TYPE}" STREQUAL "AT32F407VGT7")
    set(linker_script_SRC ${AT32_HAL_PATH}/ldscript/AT32F407xG_FLASH.ld)
elseif("${MCU_TYPE}" STREQUAL "AT32F403ACGT7")
    set(linker_script_SRC ${AT32_HAL_PATH}/ldscript/AT32F403AxG_FLASH.ld)
endif()

# Compiler options
set(compiler_OPTS
    -fdata-sections
    -ffunction-sections
    -fno-common
    -fmessage-length=0
)

# Linker options
set(linker_OPTS
    -Wl,--gc-sections
    -Wl,--no-warn-rwx-segments
)

# Create executable
add_executable(${CMAKE_PROJECT_NAME} ${sources_SRCS})

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${include_DIRS})

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE 
    ${symbols_SYMB}
    $<$<CONFIG:Debug>:DEBUG>
)

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:
        -fno-rtti
        -fno-exceptions
        -fno-threadsafe-statics
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${linker_script_SRC}
    ${cpu_PARAMS}
    ${linker_OPTS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float
    --specs=nosys.specs
    --specs=nano.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# Post-build commands
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Building HEX and BIN files"
)

