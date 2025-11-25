# cmake_minimum_required(VERSION 3.15)

# set(WINUSB_SRC
#   ${CMAKE_CURRENT_SOURCE_DIR}/winusb_class.c
#   ${CMAKE_CURRENT_SOURCE_DIR}/winusb_desc.c
# )

# set(WINUSB_HEAD
#   ${CMAKE_CURRENT_SOURCE_DIR}/winusb_class.h
#   ${CMAKE_CURRENT_SOURCE_DIR}/winusb_desc.h
# )

# add_library(WINUSB_LIB ${WINUSB_SRC} ${WINUSB_HEAD})

# target_include_directories(WINUSB_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS})

# target_compile_options(WINUSB_LIB PUBLIC 
#   -mcpu=cortex-m4 
#   -mthumb 
# )