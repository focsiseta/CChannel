#include "../headers/message.h"
#include "../headers/utils.h"

#define lock(lck) pthread_mutex_lock(lck)
#define unlock(lck) pthread_mutex_unlock(lck)
#define wait(cond,lck) pthread_cond_wait(cond,lck)
#define signal(cond) pthread_cond_signal(cond)
#define broadcast(cond) pthread_cond_broadcast(cond)

Ch * create_channel(void){
    Ch * channel = (Ch *) safe_malloc(sizeof(Ch));
    pthread_cond_init( &(channel->empty),NULL);
    pthread_cond_init( &(channel->notEmpty),NULL);
    pthread_mutex_init(&(channel->read),NULL);
    pthread_mutex_init(&(channel->write),NULL);
    pthread_mutex_init(&(channel->atomicOP),NULL);
    channel->isFull = 0;
    return channel;
}

void atomic_change_read(Ch *channel){
    lock(&(channel->atomicOP));
    channel->isFull = 0;
    channel->ptrData = NULL;
    unlock(&(channel->atomicOP));

}
void atomic_change_write(Ch *channel,Data *obj){
    lock(&(channel->atomicOP));
    channel->isFull = 1;
    channel->ptrData = obj;
    unlock(&(channel->atomicOP));

}



Data *wrap(void *arg){
    Data * obj = (Data *) safe_malloc(sizeof(Data));
    obj->toSend = arg;
    return obj;
}

void * unwrap(Data *obj){
    if(obj == NULL){
        errno = ENOENT; //No usable entity
        return NULL;
    }
    return (void *) obj->toSend;
}

void * unwrap_and_free(Data *obj){
    void * cont = unwrap(obj);
    safe_free(obj);
    return cont;
}

Data * ch_write(Ch *channel,Data *obj, int do_broadcast){
    if (channel == NULL){
        errno = EFAULT;
        return NULL;
    }
    lock(&channel->write);
    //Aspetto che il canale sia vuoto per inviare
    while(channel->isFull != 0){
        wait(&channel->empty,&channel->write);
    }

    atomic_change_write(channel,obj);
    unlock(&channel->write);
    if (do_broadcast == BROADCAST) {
        broadcast(&channel->notEmpty);
    }else {
        signal(&channel->notEmpty);
    }
    return obj;
}

Data * ch_read(Ch *channel,Data **obj){

    if (channel == NULL){
        errno = EFAULT;
        return NULL;
    }
    lock(&channel->read);
    //w8
    while(channel->isFull == 0){
        wait(&channel->notEmpty,&channel->read);
    }
    if ((obj))
        *obj = (channel->ptrData);
    atomic_change_read(channel);
    unlock(&channel->read);
    signal(&channel->empty);
    if (!(obj))
        return NULL;
    return *obj;
}

Data * empty_container(void){
    Data * container = (Data *) safe_malloc(sizeof(Data));
    container->toSend = NULL;
    container->next = NULL;
    container->prev = NULL;
    return container;
}


