#include "headers/message.h"
#include "headers/utils.h"



void *reader(void *arg){
    Ch *rd_channel = (Ch *) arg;
    Data *recvedData;
    ch_read(rd_channel,&recvedData);
    char * string = unwrap(recvedData);
    puts(string);
    return NULL;
}

void *writer(void *arg){
    Ch *wr_channel = (Ch *) arg;
    //Crafting data 
    char * string = (char *) safe_malloc((sizeof(char) * strlen("Dati sensibili"))+1);
    sprintf(string,"Dati sensibili");
    Data * tmp = wrap((void *)string);
    ch_write(wr_channel,tmp);
    return NULL;
}


int main(int argc, char const *argv[])
{
    Ch *channel = create_channel();
    pthread_t thdrs[10];
    pthread_create(&thdrs[0],NULL,writer,(void *)channel);
    pthread_detach(thdrs[0]);
    pthread_create(&thdrs[2],NULL,writer,(void *)channel);
    pthread_detach(thdrs[2]);
    pthread_create(&thdrs[1],NULL,reader,(void *)channel);
    pthread_create(&thdrs[3],NULL,reader,(void *)channel);

    pthread_join(thdrs[1],NULL);
    pthread_join(thdrs[3],NULL);
    

    return 0;
}
