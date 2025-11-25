cmake_minimum_required(VERSION 3.15)

set(NAME_LIB LSeggerRTT)

add_library(${NAME_LIB} STATIC)

if(NOT RTT_COMPILER)
  message(WARNING 
  "Please choose a compiler:\n" 
  "\t- GCC  \n"
  "\t- IAR  \n"
  "\t- KEIL \n"
  "\t- SES  \n"
  )
endif()

if ("${RTT_COMPILER}" STREQUAL "GCC")
  message(STATUS "The " ${RTT_COMPILER} " compiler was selected for the RTT library")
  set(PATCH_COMPILER "Syscalls/SEGGER_RTT_Syscalls_GCC.c")
elseif ("${RTT_COMPILER}" STREQUAL "IAR")
  message(STATUS "The " ${RTT_COMPILER} " compiler was selected for the RTT library")
  set(PATCH_COMPILER "Syscalls/SEGGER_RTT_Syscalls_IAR.c")
elseif ("${RTT_COMPILER}" STREQUAL "KEIL")
  message(STATUS "The " ${RTT_COMPILER} " compiler was selected for the RTT library")
  set(PATCH_COMPILER "Syscalls/SEGGER_RTT_Syscalls_KEIL.c")
elseif ("${RTT_COMPILER}" STREQUAL "SES")
  message(STATUS "The " ${RTT_COMPILER} " compiler was selected for the RTT library")
  set(PATCH_COMPILER "Syscalls/SEGGER_RTT_Syscalls_SES.c")
endif ()

target_include_directories(${NAME_LIB} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/Config
  ${CMAKE_CURRENT_SOURCE_DIR}/RTT
)

file(GLOB_RECURSE sources_SRCS
  "Config/*.c"
  "RTT/*.c"
  "RTT/SEGGER_RTT_ASM_ARMv7M.S"
  ${PATCH_COMPILER}
)

target_sources(${NAME_LIB} PUBLIC ${sources_SRCS})

# Внешнее подключение::
# if(GLOB_Definition_symb MATCHES "-DGLOB_USE_RTT")
# set(RTT_COMPILER "GCC")
# add_subdirectory(Middlewares/SeggerRTT)
# target_compile_options(LSeggerRTT PRIVATE
#     ${cpu_PARAMS}
#     ${compiler_OPTS}
#     ### Gnu/Clang C Options
#     $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

#     $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
#     $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
#     $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wpedantic>
#     $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
#     $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wconversion>
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>

#     # Suppressions required to build clean with clang.
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-unused-macros>
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-padded>
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-missing-variable-declarations>
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-covered-switch-default>
#     $<$<COMPILE_LANG_AND_ID:C,Clang>:-Wno-cast-align> 

#     $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
# )
# target_link_libraries(${CMAKE_PROJECT_NAME} LSeggerRTT)
# endif()