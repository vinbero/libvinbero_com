add_library(vinbero_common SHARED vinbero_common_Log.c vinbero_common_Config.c vinbero_common_Module.c)
list(APPEND vinbero_common_public_headers "vinbero_common_Module.h")
list(APPEND vinbero_common_public_headers "vinbero_common_TlModule.h")
list(APPEND vinbero_common_public_headers "vinbero_common_ClModule.h")
list(APPEND vinbero_common_public_headers "vinbero_common_Config.h")
list(APPEND vinbero_common_public_headers "vinbero_common_Log.h")
list(APPEND vinbero_common_public_headers "vinbero_common_Error.h")
list(APPEND vinbero_common_public_headers "vinbero_common_Status.h")
list(APPEND vinbero_common_public_headers "vinbero_common_Dlsym.h")
list(APPEND vinbero_common_public_headers "vinbero_common_Call.h")
file(GLOB interfaces "vinbero_interface_*.h")

install(FILES ${interfaces} DESTINATION include/vinbero)
set_target_properties(vinbero_common PROPERTIES VERSION ${vinbero_common_version}
                                                SOVERSION ${vinbero_common_soversion}
                                                PUBLIC_HEADER "${vinbero_common_public_headers}")

target_link_libraries(vinbero_common jansson fastdl)
install(TARGETS vinbero_common
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/vinbero_common)
