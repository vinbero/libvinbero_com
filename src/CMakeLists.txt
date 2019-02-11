add_library(vinbero_com SHARED vinbero_com_Log.c vinbero_com_Config.c vinbero_com_Object.c)
list(APPEND vinbero_com_public_headers "vinbero_com_Module.h")
list(APPEND vinbero_com_public_headers "vinbero_com_TlModule.h")
list(APPEND vinbero_com_public_headers "vinbero_com_ClModule.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Config.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Log.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Error.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Status.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Dlsym.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Call.h")
list(APPEND vinbero_com_public_headers "vinbero_com_Object.h")

set_target_properties(vinbero_com PROPERTIES VERSION ${vinbero_com_version}
                                                SOVERSION ${vinbero_com_soversion}
                                                PUBLIC_HEADER "${vinbero_com_public_headers}")

target_link_libraries(vinbero_com jansson fastdl)
install(TARGETS vinbero_com
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/vinbero_com)
