#include "../headers/message.h"
#include "../headers/utils.h"

#define lock(lck) pthread_mutex_lock(lck)
#define unlock(lck) pthread_mutex_unlock(lck)
#define wait(cond,lck) pthread_cond_wait(cond,lck)
#define signal(cond) pthread_cond_signal(cond)

Ch * create_channel(void){
    Ch * channel = (Ch *) safe_malloc(sizeof(Ch));
    pthread_cond_init( &(channel->empty),NULL);
    pthread_cond_init( &(channel->notEmpty),NULL);
    pthread_mutex_init(&(channel->read),NULL);
    pthread_mutex_init(&(channel->write),NULL);
    return channel;
}

Data *wrap(void *arg){
    Data * obj = (Data *) safe_malloc(sizeof(Data));
    obj->toSend = arg;
    return obj;
}

void * unwrap(Data *obj){
    if(obj == NULL || obj->toSend == NULL ){
        errno = ENOENT; //No usable entity
        return NULL;
    }
    return (void *) obj->toSend;
}

Data * ch_write(Ch *channel,Data *obj){
    if (channel == NULL){
        errno = EFAULT;
        return NULL;
    }
    lock(&channel->write);
    //Aspetto che il canale sia vuoto per inviare
    while(channel->ptrData != NULL){
        wait(&channel->empty,&channel->write);
    }

    channel->ptrData = obj;
    signal(&channel->notEmpty);
    unlock(&channel->write);
    return obj;
}

Data * ch_read(Ch *channel,Data **obj){

    if (channel == NULL){
        errno = EFAULT;
        return NULL;
    }
    lock(&channel->read);
    //w8
    while(channel->ptrData == NULL){
        wait(&channel->notEmpty,&channel->read);
    }
    if ((obj))
        *obj = (channel->ptrData);
    channel->ptrData = NULL;
    signal(&channel->empty);
    unlock(&channel->read);
    if (!(obj))
        return NULL;
    return *obj;

    

}