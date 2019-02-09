#include <dlfcn.h>
#include "vinbero_com_Config.h"
#include "vinbero_com_Error.h"
#include "vinbero_com_Status.h"
#include "vinbero_com_Module.h"

void vinbero_com_Module_init(struct vinbero_com_Module* module, const char* name, const char* version, bool childrenRequired) {
    module->name = name;
    module->version = version;
    module->childrenRequired = childrenRequired;
}
