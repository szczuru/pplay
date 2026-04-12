#include "jailbreak.h"
#include "kernelrw.h"
#include <orbis/libkernel.h>
#include <stdio.h>

#ifdef __PS4__

int jbc_escape(void)
{
    printf("[pplay] === Starting FULL sandbox escape (libjbc) ===\n");

    if (kernelrw_init() != 0) {
        printf("[pplay] kernelrw_init failed\n");
        return -1;
    }

    if (jbc_escape_internal() != 0) {   // wewnętrzna funkcja z libjbc
        printf("[pplay] jbc_escape_internal failed\n");
        return -1;
    }

    printf("[pplay] SANDBOX ESCAPE SUCCESS! Full filesystem access granted.\n");
    return 0;
}

#endif
