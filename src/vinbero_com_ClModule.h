#ifndef _VINBERO_COM_CLMODULE_H
#define _VINBERO_COM_CLMODULE_H

#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>

struct vinbero_com_ClModule {
    struct vinbero_com_TlModule* tlModule;
    union genc_Generic localClModule;
    void* arg;
    GENC_TREE_NODE(struct vinbero_com_ClModule, struct vinbero_com_ClModule*);
};

#endif
