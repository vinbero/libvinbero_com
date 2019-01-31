cmake_minimum_required(VERSION 2.8)
project(libvinbero_common C)

if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
  endif()
else()
  set(CMAKE_C_STANDARD 11)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(vinbero_common_version "0.3.0")
set(vinbero_common_soversion "0.3")

option(BUILD_WITH_COVERAGE "Build with coverage" OFF)
if(${BUILD_WITH_COVERAGE})
  message("** Build with coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMAKE_BUILD_TYPE" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "MinSizeRel" "Release" "RelWithDebInfo")
endif()

message("** Build type is ${CMAKE_BUILD_TYPE}")

execute_process(COMMAND sh "-c" "git branch -a | grep remotes/origin/master | wc -l" ERROR_QUIET OUTPUT_VARIABLE master_branch_exists)
if(${master_branch_exists} EQUAL 1)
  message("** Branch origin/master exists, enable automatic AUTHORS generation")
  execute_process(COMMAND git shortlog -nse --no-merges origin/master OUTPUT_FILE "${CMAKE_SOURCE_DIR}/AUTHORS")
  add_custom_target(authors ALL git shortlog -nse --no-merges origin/master > "${CMAKE_SOURCE_DIR}/AUTHORS")
endif()

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
