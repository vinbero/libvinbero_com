cmake_minimum_required(VERSION 2.8)
project(libvinbero_common C)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(vinbero_common_version "0.3.0")
set(vinbero_common_soversion "0.3")

if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS}")
  endif()
else()
  set(CMAKE_C_STANDARD 11)
endif()

execute_process(COMMAND git remote set-branches origin '*')
execute_process(COMMAND git fetch)
execute_process(COMMAND git shortlog -nse --no-merges origin/master OUTPUT_FILE "${CMAKE_SOURCE_DIR}/AUTHORS")
add_custom_target(authors ALL git shortlog -nse --no-merges origin/master > "${CMAKE_SOURCE_DIR}/AUTHORS")

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
