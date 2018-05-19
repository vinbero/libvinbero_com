#include "vinbero_common_Args.h"
#include <stdlib.h>
#include <stdbool.h>
bool vinbero_common_Args_checkCount(void* args[], int count) {
    for(int index = 0; index < count; ++index) {
        if(args[index] == NULL)
            return false;
    }
    return true;
}
