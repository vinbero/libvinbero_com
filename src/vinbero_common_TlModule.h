#ifndef _VINBERO_COMMON_TLMODULE_H
#define _VINBERO_COMMON_TLMODULE_H

#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>

struct vinbero_common_TlModule {
    struct vinbero_common_Module* module;
    union genc_Generic localTlModule;
    void* arg;
    GENC_TREE_NODE(struct vinbero_common_TlModule, struct vinbero_common_TlModule*);
};

#endif
