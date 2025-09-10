cmake_minimum_required(VERSION 3.22)

if(NOT usb_class)
  message(FATAL_ERROR " usb_class not defined!")
  return()
endif()

# add_subdirectory(usbd_drivers)
set(USBD_DRV_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/src/usbd_core.c
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/src/usbd_int.c
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/src/usbd_sdr.c
)

# file(USBD_DRV_SRC "usbd_drivers/src/*.c")

set(USBD_DRV_HEAD
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/inc/usb_std.h
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/inc/usbd_core.h
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/inc/usbd_int.h
  ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/inc/usbd_sdr.h
)
add_library(USB_LIBS ${USBD_DRV_SRC} ) #${USBD_DRV_HEAD}
# target_include_directories(USB_LIBS PUBLIC ${USBD_DRV_SRC} ${USBD_DRV_HEAD})
# link_directories(USB_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/inc/)
include_directories(USB_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/usbd_drivers/inc/)
target_sources(USB_LIBS PUBLIC ${USBD_DRV_SRC})


if(usb_class STREQUAL "USB_AUDIO")
  message("Connect USB_Audio lib")

  elseif(usb_class STREQUAL "USB_AUDIO_HID")
  message("Connect USB_AUDIO_HID lib")

  elseif(usb_class STREQUAL "USB_WINDRV")
  message("Connect USB_WINDRV lib")
  # add_subdirectory(cdc)
  # set(WINUSB_DIR
  #   ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/
  # )
  set(WINUSB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/winusb_class.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/winusb_desc.c
  )

  # set(WINUSB_HEAD
  #   ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/winusb_class.h
  #   ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/winusb_desc.h
  # )
  # add_subdirectory(usbd_class/winusb)
  # target_include_directories(USB_LIBS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/)
  # target_link_directories(USB_LIBS PUBLIC ${WINUSB_SRC} ${WINUSB_HEAD})
  include_directories(USB_LIBS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/usbd_class/winusb/)
  target_sources(USB_LIBS PUBLIC ${WINUSB_SRC})
endif()

# 
#
# Add Config File
include_directories(USB_LIBS
    INTERFACE
        .
        # Note: DEPRECATED but still supported, may be removed in a future release.
        $<$<NOT:$<TARGET_EXISTS:usb_config>>:${USB_CONFIG_FILE_DIRECTORY}>
)

link_libraries(USB_LIBS
    INTERFACE
        $<$<TARGET_EXISTS:usb_config>:usb_config>
)


# ****************************************************************************************************************************
# ******************************************************  EXAMPLE  ***********************************************************
# ****************************************************************************************************************************

# cmake_minimum_required(VERSION 3.15)

# project(FreeRTOS10)

# set(INCLUDE_DIRS 
#   ${CMAKE_CURRENT_SOURCE_DIR}/config
#   ${CMAKE_CURRENT_SOURCE_DIR}/source/include
#   ${CMAKE_CURRENT_SOURCE_DIR}/source/portable/GCC/ARM_CM4F/
# )

# set(FRTOS_SOURCES 
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/portable/common/mpu_wrappers.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/portable/GCC/ARM_CM4F/port.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/portable/memmang/heap_1.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/croutine.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/event_groups.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/list.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/queue.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/stream_buffer.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/tasks.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/timers.c
# )

# set(FRTOS_HEADERS 
#     ${CMAKE_CURRENT_SOURCE_DIR}/config/FreeRTOSConfig.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/FreeRTOS.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/atomic.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/croutine.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/deprecated_definitions.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/event_groups.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/list.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/message_buffer.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/mpu_prototypes.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/mpu_wrappers.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/portable.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/projdefs.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/queue.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/semphr.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/stack_macros.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/StackMacros.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/stream_buffer.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/task.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/include/timers.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/source/portable/GCC/ARM_CM4F/portmacro.h
# )



# add_library(usb_class_lib ${FRTOS_SOURCES} ${FRTOS_HEADERS})

# target_include_directories(usb_class_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS})

# target_compile_options(usb_class_lib PRIVATE
#   -mcpu=cortex-m4 
#   -mthumb 
# )