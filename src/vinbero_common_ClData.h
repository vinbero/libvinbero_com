#ifndef _VINBERO_COMMON_CLDATA_H
#define _VINBERO_COMMON_CLDATA_H

#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>

struct vinbero_common_ClData {
    union genc_Generic generic; // Should be renamed to object?
    void* arg;
    GENC_TREE_NODE(struct vinbero_common_ClData, struct vinbero_common_ClData*);
};

#endif
