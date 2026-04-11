#include "jailbreak.h"
#include "kernelrw.h"
#include "utils.h"
#include <orbis/libkernel.h>
#include <stdio.h>

#ifdef __PS4__

int jailbreak_escape(void)
{
    printf("pplay: Starting sandbox escape...\n");

    // Podstawowa ucieczka z sandboxa - kopiujemy prison i rootdir z init (PID 1)
    if (kernelrw_init() != 0) {
        printf("pplay: kernelrw_init failed\n");
        return -1;
    }

    // Główna funkcja escape (z oryginalnego libjbc)
    if (jbc_escape() != 0) {          // to jest główna funkcja z repo
        printf("pplay: jbc_escape failed\n");
        return -1;
    }

    printf("pplay: Sandbox escape successful! Full filesystem access granted.\n");
    return 0;
}

#endif
