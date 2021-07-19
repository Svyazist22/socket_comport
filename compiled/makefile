OBJECTS = stou.o utos.o fifo.o logger.o uart.o server.o client.o crypt.o
MAIN = ../apps/project/
LIBS = ../libs/
INC = ../include/
CP = cross_platform/
LINUX = linux/
ADDIT = additional/
COM = com_port/
TL = transport_layer/

CFLAGS = -c -Wall
STOU = ../apps/StoU/
UTOS = ../apps/UtoS/


all: utos stou 


stou: stou.o server.o logger.o uart.o fifo.o crypt.o
	g++ stou.o logger.o server.o uart.o fifo.o crypt.o -o stou

utos: utos.o client.o logger.o uart.o fifo.o crypt.o
	g++ utos.o client.o logger.o uart.o fifo.o crypt.o -o utos


stou.o: $(STOU)stou.cpp $(LIBS)$(LINUX)$(TL)server.cpp $(LIBS)$(LINUX)$(COM)uart.cpp
	g++ $(CFLAGS) $(STOU)stou.cpp $(LIBS)$(LINUX)$(TL)server.cpp $(LIBS)$(LINUX)$(COM)uart.cpp

utos.o: $(UTOS)utos.cpp $(LIBS)$(LINUX)$(TL)client.cpp 
	g++ $(CFLAGS) $(UTOS)utos.cpp $(LIBS)$(LINUX)$(TL)client.cpp

fifo.o: $(LIBS)$(CP)fifo.c
	g++ $(LIBS)$(CP)fifo.c

logger.o: $(LIBS)$(LINUX)$(ADDIT)logger.cpp
	g++ $(CFLAGS) $(LIBS)$(LINUX)$(ADDIT)logger.cpp

uart.o: $(LIBS)$(LINUX)$(COM)uart.cpp $(LIBS)$(CP)fifo.c $(LIBS)$(LINUX)$(ADDIT)logger.cpp $(LIBS)$(CP)crypt.c
	g++ $(CFLAGS) $(LIBS)$(LINUX)$(COM)uart.cpp $(LIBS)$(CP)fifo.c $(LIBS)$(LINUX)$(ADDIT)logger.cpp $(LIBS)$(CP)crypt.c

client.o: $(LIBS)$(LINUX)$(TL)client.cpp $(LIBS)$(LINUX)$(ADDIT)logger.cpp
	g++ $(CFLAGS) $(LIBS)$(LINUX)$(TL)client.cpp $(LIBS)$(LINUX)$(ADDIT)logger.cpp

server.o: $(LIBS)$(LINUX)$(TL)server.cpp $(LIBS)$(LINUX)$(ADDIT)logger.cpp
	g++ $(CFLAGS) $(LIBS)$(LINUX)$(TL)server.cpp $(LIBS)$(LINUX)$(ADDIT)logger.cpp

crypt.o: $(LIBS)$(CP)crypt.c
	g++ $(CFLAGS) $(LIBS)$(CP)crypt.c



clean:
		rm utos stou $(OBJECTS)

.PHONY : all clean