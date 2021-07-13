/**
 * @file uart.cpp
 * @author Vlad Bakanov
 * @brief Работа с USB<=>UART
 * @version 0.1
 * @date 2021-06-28
 */
#include "../include/uart.h"
#include <fcntl.h>
#include <termios.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>

Logger logg;



void Uart::uart_init(int fd){

    struct termios tty;
    
    tcgetattr(fd,&tty);        // Получение стандартных настроек

    cfsetspeed(&tty, B9600);   // Скорость приема и передачи

    tty.c_cflag     &=  ~PARENB;            
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    tty.c_cflag     &=  ~CRTSCTS;           
    tty.c_cc[VMIN]   =  0;                  
    tty.c_cc[VTIME]  =  0;                  
    tty.c_cflag     |=  CREAD | CLOCAL;     

    cfmakeraw(&tty);

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
    {
        logg.err("Unable to set port parameters: %s",strerror(errno));     
    }
}


void Uart::uart_receive(int fd, fifo_t *buf)
{
    char response[1024];
    memset(response, '\0', sizeof(response));

    //fcntl(fd, F_SETFL, FNDELAY);
    read(fd, response, 1024);

    //tcflush( fd, TCIFLUSH );

    if (fifo_free(buf)>=1024)
    {
        fifo_write_push(buf,response,1024);        
    }
    else
    {
        logg.err("The buffer is ful");
    }
}

void Uart::uart_transmit(int fd, char* str,size_t size)
{
    if (write(fd,str,size) == -1)
    {
        logg.err("Write error! %s",strerror(errno));
    }
    else
    {
        logg.info("The message was sent to the UART");
    }
}

