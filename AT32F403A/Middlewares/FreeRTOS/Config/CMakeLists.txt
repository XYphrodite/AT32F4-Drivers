# FreeRTOS internal cmake file. Do not use it in user top-level project

# add_library(freertos_config INTERFACE)

# target_include_directories(freertos_config
#     INTERFACE
#         .
#         # Note: DEPRECATED but still supported, may be removed in a future release.
#         $<$<NOT:$<TARGET_EXISTS:freertos_config>>:${FREERTOS_CONFIG_FILE_DIRECTORY}>
# )