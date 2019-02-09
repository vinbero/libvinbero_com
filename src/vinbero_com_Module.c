#include <dlfcn.h>
#include "vinbero_common_Config.h"
#include "vinbero_common_Error.h"
#include "vinbero_common_Status.h"
#include "vinbero_common_Module.h"

void vinbero_common_Module_init(struct vinbero_common_Module* module, const char* name, const char* version, bool childrenRequired) {
    module->name = name;
    module->version = version;
    module->childrenRequired = childrenRequired;
}
