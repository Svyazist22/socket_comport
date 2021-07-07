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

    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  0;                  // read doesn't block
    tty.c_cc[VTIME]  =  0;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

cfmakeraw(&tty);

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
    {
        logg.err("Unable to set port parameters");     
    }
}


void Uart::uart_receive(int fd, fifo_t buf)
{
    int n = 0;
    int num = 0;
    char hash[8];
    char response[1024];
    memset(response, '\0', sizeof(response));
    char symbol = '\0';
    fcntl(fd, F_SETFL, FNDELAY);
    read(fd, response, 1024);
    logg.info("Message received:%s",response);
    tcflush( fd, TCIFLUSH );
    get_hash(response,strlen(response),hash);
    logg.info("Hash:%s",hash);
}

void Uart::uart_transmit(int fd, char* str,size_t size)
{
    if (write(fd,str,size) == -1)
    {
        logg.err("Write error!");
    }

}

