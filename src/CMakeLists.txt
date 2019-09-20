add_library(vinbero_com SHARED)
target_sources(vinbero_com PRIVATE vinbero_com_Log.c vinbero_com_Config.c vinbero_com_Object.c)
list(APPEND public_headers "vinbero_com_Module.h")
list(APPEND public_headers "vinbero_com_TlModule.h")
list(APPEND public_headers "vinbero_com_ClModule.h")
list(APPEND public_headers "vinbero_com_Config.h")
list(APPEND public_headers "vinbero_com_Log.h")
list(APPEND public_headers "vinbero_com_Error.h")
list(APPEND public_headers "vinbero_com_Status.h")
list(APPEND public_headers "vinbero_com_Dlsym.h")
list(APPEND public_headers "vinbero_com_Call.h")
list(APPEND public_headers "vinbero_com_Object.h")

target_compile_options(vinbero_com PRIVATE -Wall)
set_target_properties(
    vinbero_com
    PROPERTIES
    C_STANDARD 11
    VERSION ${version}
    SOVERSION ${soversion}
    PUBLIC_HEADER "${public_headers}"
)

target_link_libraries(vinbero_com jansson fastdl yaml)

install(
    TARGETS vinbero_com
    EXPORT vinbero_com-targets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/vinbero_com
)

install(
    EXPORT vinbero_com-targets
    NAMESPACE vinbero_com::
    FILE vinbero_com-config.cmake
    DESTINATION lib/cmake/vinbero_com
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    vinbero_com-config-version.cmake
    VERSION ${version}
    COMPATIBILITY SameMinorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/vinbero_com-config-version.cmake
    DESTINATION lib/cmake/vinbero_com
)
