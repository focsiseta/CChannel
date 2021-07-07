#include "../headers/utils.h"

void * safe_malloc(size_t __size){
    void * buffer = malloc(__size);
    if (buffer == NULL){
        exit(EXIT_FAILURE);
    }
    memset(buffer,0,__size);
    return buffer;
}