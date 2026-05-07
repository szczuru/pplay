#include "multi-jb.h"
#include <stdio.h>

#ifdef __PS4__

int multi_jb(void)
{
    printf("[pplay] multi-jb (Itemzflow style) started...\n");

    // Główna funkcja escape z Itemzflow
    int ret = syscall(0x1A, 1, 0, 0, 0, 0, 0);   // prison
    if (ret == 0) printf("[pplay] Prison escape OK\n");

    ret = syscall(0x4B, 0, 0, 0, 0, 0, 0);       // rootdir
    if (ret == 0) printf("[pplay] Rootdir escape OK\n");

    printf("[pplay] multi-jb finished - full root should be active.\n");
    return 0;
}

#endif
