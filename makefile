OBJS	= ./bin/message.o ./bin/utils.o 
SOURCE	= ./src/message.c ./src/utils.c main.c
HEADER	= ./headers/message.h ./headers/utils.h
OUT	= ./bin/test
CC	 = gcc
FLAGS	 = -g -c -Wall -lpthread
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) main.c -o $(OUT) $(LFLAGS)

./bin/utils.o: ./src/utils.c
	$(CC) $(FLAGS) ./src/utils.c -std=gnu11 && mv *.o ./bin/

./bin/message.o: ./src/message.c
	$(CC) $(FLAGS) ./src/message.c -std=gnu11 && mv *.o ./bin/

clean:
	rm -f $(OBJS) $(OUT)