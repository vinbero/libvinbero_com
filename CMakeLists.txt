cmake_minimum_required(VERSION 2.8)
project(libvinbero_common C)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(vinbero_common_version "0.3.0")
set(vinbero_common_soversion "0.3")

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "--coverage ${CMAKE_C_FLAGS}")

if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS}")
  endif()
else()
  set(CMAKE_C_STANDARD 11)
endif()


execute_process(COMMAND sh "-c" "git branch -a | grep remotes/origin/master | wc -l" ERROR_QUIET OUTPUT_VARIABLE master_branch_exists)
if(${master_branch_exists} EQUAL 1)
  message("Branch origin/master exists, enabling automatic AUTHORS generation")
  execute_process(COMMAND git shortlog -nse --no-merges origin/master OUTPUT_FILE "${CMAKE_SOURCE_DIR}/AUTHORS")
  add_custom_target(authors ALL git shortlog -nse --no-merges origin/master > "${CMAKE_SOURCE_DIR}/AUTHORS")
endif()

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
