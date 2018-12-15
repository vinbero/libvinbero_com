cmake_minimum_required(VERSION 2.8)
project(libvinbero_common C)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(vinbero_common_version "0.2.0")
set(vinbero_common_soversion "0.2")

if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS}")
  endif()
else()
  set(CMAKE_C_STANDARD 11)
endif()

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
