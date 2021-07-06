OBJECTS = stou.o utos.o fifo.o logger.o uart.o server.o client.o uart.o
MAIN = ../apps/project/
LIBS = ../libs/
INC = ../include/
CFLAGS = -c -Wall
STOU = ../apps/StoU/
UTOS = ../apps/UtoS/


all: utos stou 


stou: stou.o server.o logger.o uart.o fifo.o
	g++ stou.o logger.o server.o uart.o fifo.o -o stou

utos: utos.o client.o logger.o uart.o fifo.o
	g++ utos.o client.o logger.o uart.o fifo.o -o utos


stou.o: $(STOU)stou.cpp $(LIBS)server.cpp $(LIBS)uart.cpp
	g++ $(CFLAGS) $(STOU)stou.cpp $(LIBS)server.cpp $(LIBS)uart.cpp

utos.o: $(UTOS)utos.cpp $(LIBS)client.cpp
	g++ $(CFLAGS) $(UTOS)utos.cpp $(LIBS)client.cpp

fifo.o: $(LIBS)fifo.c
	g++ $(CFLAGS) $(LIBS)fifo.c

logger.o: $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)logger.cpp

uart.o: $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp

client.o: $(LIBS)client.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)client.cpp $(LIBS)logger.cpp

server.o: $(LIBS)server.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)server.cpp $(LIBS)logger.cpp



clean:
		rm utos stou $(OBJECTS)

.PHONY : all clean