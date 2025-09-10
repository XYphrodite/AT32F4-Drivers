message(STATUS "Received ${MCU_TYPE}")

# file(GLOB_RECURSE sources_SRCS
#     "${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support/*.c"
#     "${SHARED_LIB_PATH}/AT32F403A/drivers/src/*.c"
#     "${SHARED_LIB_PATH}/AT32F403A/startup/startup_at32f403a_407.s"
#     "${SHARED_LIB_PATH}/Library/ArteryCore/*.cpp"
#     "${SHARED_LIB_PATH}/Library/ArteryCore/*.c"
#     "${SHARED_LIB_PATH}/Library/Command/*.c"
#     "${SHARED_LIB_PATH}/Library/Command/*.cpp"
#     "${SHARED_LIB_PATH}/Library/CRC/*.c"
#     "${SHARED_LIB_PATH}/Library/CRC/*.cpp"
# )

# set(include_c_DIRS ${include_c_DIRS}
#     ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/core_support
#     ${SHARED_LIB_PATH}/AT32F403A/cmsis/cm4/device_support
#     ${SHARED_LIB_PATH}/AT32F403A/drivers/inc
#     ${SHARED_LIB_PATH}/Library/ArteryCore
#     ${SHARED_LIB_PATH}/Library/Command
#     ${SHARED_LIB_PATH}/Library
#     ${SHARED_LIB_PATH}/Library/CRC
# )


message(STATUS "Shared path: ${SHARED_LIB_PATH}")
message(STATUS "cmake_shared.cmake included successfully")
message(STATUS "Include Paths: ${include_c_DIRS}")


# message(STATUS "Include Paths: ${include_c_DIRS}")

