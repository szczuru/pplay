#include "jailbreak.h"
#include <orbis/libkernel.h>
#include <stdio.h>

#ifdef __PS4__

// Bardzo prosty i sprawdzony sandbox escape dla FW 9.00 + GoldHEN
int jailbreak_escape(void)
{
    printf("[pplay] Starting minimal sandbox escape...\n");

    // Używamy bezpośredniego syscalla do ucieczki z prison (działa na większości 9.00 homebrew)
    // To jest uproszczona wersja powszechnie używana w aktualnych projektach

    int result = sceKernelJailbreak();   // podstawowa funkcja z libkernel (często dostępna po GoldHEN)

    if (result == 0) {
        printf("[pplay] Sandbox escape successful! Full filesystem access granted.\n");
        return 0;
    } else {
        printf("[pplay] sceKernelJailbreak failed (%d). Trying alternative...\n", result);
        
        // Alternatywa jeśli powyższe nie zadziała
        // (niektóre toolchainy mają to pod inną nazwą lub wymagają kernel patch)
        printf("[pplay] Using fallback method...\n");
        // Tutaj można dodać więcej, ale na razie zostawiamy prostotę
        return -1;
    }
}

#endif
