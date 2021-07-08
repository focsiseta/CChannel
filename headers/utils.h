#ifndef __UTEELS
#define __UTEELS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define safe_free(pointer) if (pointer != NULL) {free(pointer); pointer = NULL;}

void * safe_malloc(size_t);
#endif