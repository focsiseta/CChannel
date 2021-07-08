#ifndef __EGCF
#define __EGCF

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_QUEUE_NAME 64
#define BROADCAST 1

typedef struct data {

    void * toSend;
    struct data *prev;
    struct data *next;

}Data;

typedef struct blocking_queue{

    pthread_mutex_t read;
    pthread_mutex_t write;
    pthread_cond_t  empty;
    pthread_cond_t  notEmpty;
    pthread_mutex_t atomicOP;
    int isFull;
    Data * ptrData;


}Ch;

Ch *create_channel(void);
Data *wrap(void *arg);
void *unwrap(Data *);
Data * ch_write(Ch *channel,Data *obj,int do_broadcast);
Data * ch_read(Ch *channel,Data **obj);
Data * empty_container(void);


#endif