OBJECTS = stou.o utos.u fifo.o logger.o uart.o
MAIN = ../apps/project/
LIBS = ../libs/
INC = ../include/
CFLAGS = -c -Wall
STOU = ../apps/StoU/
UTOS = ../apps/UtoS/


all: utos stou 


stou: stou.o logger.o
	g++ -pthread stou.o logger.o -o stou

utos: utos.o logger.o
	g++ -pthread utos.o logger.o -o utos


stou.o: $(STOU)stou.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(STOU)stou.cpp $(LIBS)logger.cpp

utos.o: $(UTOS)utos.cpp $(LIBS)logger.cpp
	g++ $(CFLAGS) $(UTOS)utos.cpp $(LIBS)logger.cpp	

fifo.o: $(LIBS)fifo.c
	g++ $(CFLAGS) $(LIBS)fifo.c

logger.o: $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)logger.cpp

uart.o: $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp
	g++ $(CFLAGS) $(LIBS)uart.cpp $(LIBS)fifo.c $(LIBS)logger.cpp


clean:
		rm utos stou $(OBJECTS)
		
.PHONY : all clean