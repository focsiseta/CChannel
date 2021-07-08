#include "../headers/message.h"
#include "../headers/utils.h"



void *writer(void *arg){
    Ch *wr_channel = (Ch *) arg;
    //Crafting data 
    char * string = (char *) safe_malloc((sizeof(char) * strlen("Dati sensibili"))+1);
    sprintf(string,"Dati sensibili ");
    Data * tmp = wrap(string);
    ch_write(wr_channel,tmp,0);
    return NULL;
}

void *cooler_writer(void *arg){
    Ch *wr_channel = (Ch *) arg;
    //Crafting data 
    char * string = (char *) safe_malloc((sizeof(char) * strlen("Dati sensibili"))+1);
    sprintf(string,"Dati sensibili ");
    Data * tmp = wrap(string);
    ch_write(wr_channel,tmp,0);
    ch_write(wr_channel,NULL,0);
    return NULL;
}


int main(int argc, char const *argv[])
{

    //Foreach message that comes into channel, iterate
    //when a thread sends a NULL Data loop breaks

    Ch *channel = create_channel();
    pthread_t thdrs[10];
    pthread_create(&thdrs[0],NULL,writer,(void *)channel);
    pthread_detach(thdrs[0]);
    pthread_create(&thdrs[2],NULL,writer,(void *)channel);
    pthread_detach(thdrs[2]);
    pthread_create(&thdrs[3],NULL,cooler_writer,(void *)channel);
    pthread_detach(thdrs[3]);
    char * string;

    for(Data * i = empty_container();ch_read(channel,&i);){
        string = (char * ) unwrap(i);
        puts(string);
    }
    

    return 0;
}
