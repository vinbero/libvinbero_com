add_library(vinbero_log SHARED vinbero_log.c)
set_target_properties(vinbero_log PROPERTIES PUBLIC_HEADER "vinbero_log.h")
install(TARGETS vinbero_log 
        LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include)
