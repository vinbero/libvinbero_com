#include <dlfcn.h>
#include "vinbero_common_Config.h"
#include "vinbero_common_Error.h"
#include "vinbero_common_Module.h"

int vinbero_common_Module_dlopen(struct vinbero_common_Module* module) {
    const char* modulePath;
    if((modulePath = json_string_value(json_object_get(json_object_get(module->config->json, (module)->id), "path"))) == NULL)
        return VINBERO_COMMON_ERROR_INVALID_CONFIG;
    else if(fastdl_open(&(module)->dlHandle, modulePath, RTLD_LAZY | RTLD_GLOBAL) == -1)
        return VINBERO_COMMON_ERROR_DLOPEN;
    else
        return 0;
}
