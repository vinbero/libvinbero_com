add_library(vinbero_common SHARED vinbero_common_Log.c vinbero_common_Config.c vinbero_common_Module.c)
target_link_libraries(vinbero_common jansson)
list(APPEND public_headers "vinbero_common_Module.h")
list(APPEND public_headers "vinbero_common_Config.h")
list(APPEND public_headers "vinbero_common_Log.h")
list(APPEND public_headers "vinbero_common_Error.h")
list(APPEND public_headers "vinbero_common_Status.h")
list(APPEND public_headers "vinbero_common_Call.h")
list(APPEND public_headers "vinbero_common_ClModule.h")
list(APPEND public_headers "vinbero_common_Dlsym.h")
list(APPEND public_headers "vinbero_common_TlModule.h")
set_target_properties(vinbero_common PROPERTIES
                                     PUBLIC_HEADER "${public_headers}")
install(TARGETS vinbero_common
        LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include/vinbero_common)
