OBJECTS = main.o logger.o uart.o client.o server.o
MAIN = ../apps/project/
LIBS = ../libs/
INC = ../include/
CFLAGS = -c -Wall
SERVER = ../apps/server/
CLIENT = ../apps/client/


all: client server main

main: main.o 
	g++ -pthread main.o -o main

client: client.o logger.o
	g++ client.o logger.o -o client

server: server.o logger.o
	g++ -pthread server.o logger.o -o server

main.o: $(MAIN)main.cpp 
	g++ $(CFLAGS) $(MAIN)main.cpp 

client.o: $(CLIENT)client.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(CLIENT)client.cpp $(LIBS)logger.cpp

server.o: $(SERVER)server.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(SERVER)server.cpp $(LIBS)logger.cpp	

#astring.o: $(LIBS)astring.cppmain.o logger.o uart.o client.o server.o
#	g++ $(CFLAGS) $(LIBS)eventmonitor.cpp

#fifo.o: $(LIBS)fifo.c
#	g++ $(CFLAGS) $(LIBS)fifo.c

logger.o: $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)logger.cpp

#testing.o: $(LIBS)testing.cpp $(LIBS)logger.cpp $(LIBS)fifo.c
#	g++ $(CFLAGS) $(LIBS)testing.cpp $(LIBS)logger.cpp $(LIBS)fifo.c
	
#timer.o: $(LIBS)timer.cpp
#	g++ $(CFLAGS) $(LIBS)timer.cpp

uart.o: $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp

clean:
		rm client server main $(OBJECTS)
.PHONY : all clean