# cmake_minimum_required(VERSION 3.15)

# set(USBD_DRV_SRC
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/usbd_core.c
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/usbd_int.c
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/usbd_sdr.c
# )

# set(USBD_DRV_HEAD
#   ${CMAKE_CURRENT_SOURCE_DIR}/inc/usb_std.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/inc/usbd_core.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/inc/usbd_int.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/inc/usbd_sdr.h
# )

# add_library(USBD_Driver ${USBD_DRV_SRC} ${USBD_DRV_HEAD})

# target_include_directories(USBD_Driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS})

# target_compile_options(USBD_Driver PUBLIC 
#   -mcpu=cortex-m4 
#   -mthumb 
# )