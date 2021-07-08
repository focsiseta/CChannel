#include <unistd.h>
#include <string.h>
#include "../headers/message.h"
#include "../headers/utils.h"

typedef struct args{
    Ch * channel;
    int number;

}Args;


void *worker_thread(void * arg){
    Args * args = (Args *) arg;
    Ch * channel = args->channel;
    Data * container = empty_container();
    int i = 0;
    while(ch_read(channel,&container)){ // takes job until sent data = NULL
        int * job = unwrap(container);
        //work to do...
        i++;
    }
    printf("%d thread: has done %d jobs \n",args->number ,i);
}

int main(int argc, char const *argv[])
{
    Ch * channel = create_channel();
    for(int i = 0;i < 500;i++){
        pthread_t worker;
        Args * args = (Args *) safe_malloc(sizeof(Args));
        args->channel = channel;
        args->number = i;
        pthread_create(&worker,NULL,worker_thread,(void *) args);
        pthread_detach(worker);
    }

    for (int i = 0; i < 5000;i++){
        int * number = (int *) safe_malloc(sizeof(int));
        *number = i;
        Data * job = wrap(number);
        ch_write(channel,job,0);
        safe_free(number);
    }
    int wait;
    scanf("%d",&wait);
    for(int i = 0;i < 500;i++) {
        ch_write(channel, NULL, 0); //Sends 500 closing signals
        //In this case,semantically speaking, there is no difference between signal and broadcast

    }
    return 0;
}
