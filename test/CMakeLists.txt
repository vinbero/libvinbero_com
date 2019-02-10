set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)

file(GLOB test_srcs RELATIVE ${CMAKE_SOURCE_DIR}/test *.c)
foreach(test_src ${test_srcs})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} "${test_name}.c")
    target_link_libraries(${test_name} vinbero_com cmocka)
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
