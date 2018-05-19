add_library(vinbero_common SHARED vinbero_common_Args.c vinbero_common_Log.c vinbero_common_Config.c vinbero_common_Module.c)
target_link_libraries(vinbero_common jansson)
set_target_properties(vinbero_common PROPERTIES PUBLIC_HEADER "vinbero_common_Args.h;vinbero_common_Module.h;vinbero_common_Config.h;vinbero_common_Log.h;vinbero_common_Error.h")
install(TARGETS vinbero_common
        LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include/vinbero_common)
