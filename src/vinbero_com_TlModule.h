#ifndef _VINBERO_COM_TLMODULE_H
#define _VINBERO_COM_TLMODULE_H

#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>

struct vinbero_com_TlModule {
    struct vinbero_com_Module* module;
    union genc_Generic localTlModule;
    void* arg;
    int* exitEventFd;
    GENC_TREE_NODE(struct vinbero_com_TlModule, struct vinbero_com_TlModule*);
};

#endif
