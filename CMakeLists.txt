cmake_minimum_required(VERSION 3.5)
project(libvinbero_com C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

option(BUILD_WITH_COVERAGE "Build with coverage" OFF)
if(${BUILD_WITH_COVERAGE})
  message("** Build with coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(version "0.4.0")
set(soversion "0.4")

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
