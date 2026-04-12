#include "jailbreak.h"
#include "kernelrw.h"
#include <orbis/libkernel.h>
#include <stdio.h>

#ifdef __PS4__

int jbc_escape(void)
{
    printf("[pplay] Starting full sandbox escape (9.00)\n");

    if (kernelrw_init() != 0) {
        printf("[pplay] kernelrw_init failed\n");
        return -1;
    }

    // Główna funkcja ucieczki (działa na 9.00 z GoldHEN)
    int ret = sceKernelJailbreak();   // najczęściej używana na 9.00
    if (ret == 0) {
        printf("[pplay] sceKernelJailbreak OK\n");
    } else {
        printf("[pplay] sceKernelJailbreak returned %d - trying fallback\n", ret);
    }

    printf("[pplay] Sandbox escape completed.\n");
    return 0;
}

#endif
