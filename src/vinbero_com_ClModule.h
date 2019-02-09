#ifndef _VINBERO_COMMON_CLMODULE_H
#define _VINBERO_COMMON_CLMODULE_H

#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>

struct vinbero_common_ClModule {
    struct vinbero_common_TlModule* tlModule;
    union genc_Generic localClModule;
    void* arg;
    GENC_TREE_NODE(struct vinbero_common_ClModule, struct vinbero_common_ClModule*);
};

#endif
