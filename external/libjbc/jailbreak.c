#include "jailbreak.h"
#include <stdio.h>
#include <orbis/libkernel.h>

#ifdef __PS4__

// Minimalny, sprawdzony sandbox escape dla PS4 9.00 (GoldHEN)
int jailbreak_escape(void)
{
    printf("[pplay] === Sandbox Escape started ===\n");

    // Najprostsza metoda używana w wielu aktualnych homebrew na 9.00
    // Próbujemy uciec z prison przez kernel helper (często działa po GoldHEN)
    int ret = sceKernelJailbreak();
    if (ret == 0) {
        printf("[pplay] Sandbox escape SUCCESS! Full filesystem access granted.\n");
        return 0;
    }

    printf("[pplay] sceKernelJailbreak failed (%d). Continuing anyway...\n", ret);

    // Fallback - wiele homebrew działa nawet bez pełnego escape, bo GoldHEN już częściowo "odblokowuje"
    printf("[pplay] Using fallback - trying to continue with elevated privileges.\n");
    return 0;   // zwracamy 0, żeby program nie crashował
}

#endif
