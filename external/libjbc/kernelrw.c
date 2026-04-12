#include "kernelrw.h"
#include <stdio.h>

#ifdef __PS4__

int kernelrw_init(void)
{
    printf("[pplay] kernelrw initialized (stub for 9.00)\n");
    // Na 9.00 z GoldHEN kernelrw jest już w większości przypadków dostępny
    return 0;
}

#endif
