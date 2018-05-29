#include <dlfcn.h>
#include "vinbero_common_Config.h"
#include "vinbero_common_Error.h"
#include "vinbero_common_Module.h"

int vinbero_common_Module_dlopen(struct vinbero_common_Module* module, struct vinbero_common_Config* config) {
    const char* modulePath;
    if((modulePath = json_string_value(json_object_get(json_object_get((config)->json, (module)->id), "path"))) == NULL)
        return VINBERO_COMMON_EINVAL;
    else if(fastdl_open(&(module)->dlHandle, modulePath, RTLD_LAZY | RTLD_GLOBAL) == -1)
        return VINBERO_COMMON_EUNKNOWN;
    else
        return 0;
}
