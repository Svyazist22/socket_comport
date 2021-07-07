OBJECTS = stou.o utos.o fifo.o logger.o uart.o server.o client.o uart.o crypt.o
MAIN = ../apps/project/
LIBS = ../libs/
INC = ../include/
CFLAGS = -c -Wall
STOU = ../apps/StoU/
UTOS = ../apps/UtoS/


all: utos stou 


stou: stou.o server.o logger.o uart.o fifo.o crypt.o
	g++ stou.o logger.o server.o uart.o fifo.o crypt.o -o stou

utos: utos.o client.o logger.o uart.o fifo.o crypt.o
	g++ utos.o client.o logger.o uart.o fifo.o crypt.o -o utos


stou.o: $(STOU)stou.cpp $(LIBS)server.cpp $(LIBS)uart.cpp
	g++ $(CFLAGS) $(STOU)stou.cpp $(LIBS)server.cpp $(LIBS)uart.cpp

utos.o: $(UTOS)utos.cpp $(LIBS)client.cpp 
	g++ $(CFLAGS) $(UTOS)utos.cpp $(LIBS)client.cpp

fifo.o: $(LIBS)fifo.c
	g++ $(CFLAGS) $(LIBS)fifo.c

logger.o: $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)logger.cpp

uart.o: $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp $(LIBS)crypt.c
	g++ $(CFLAGS) $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp $(LIBS)crypt.c

client.o: $(LIBS)client.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)client.cpp $(LIBS)logger.cpp

server.o: $(LIBS)server.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)server.cpp $(LIBS)logger.cpp

crypt.o: $(LIBS)crypt.c
	g++ $(CFLAGS) $(LIBS)crypt.c



clean:
		rm utos stou $(OBJECTS)

.PHONY : all clean