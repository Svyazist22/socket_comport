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

Logger logg;


void Uart::uart_init(int fd){

    struct termios tty;
    
    tcgetattr(fd,&tty);        // Получение стандартных настроек

    cfsetspeed(&tty, B9600);   // Скорость приема и передачи

    tty.c_cflag     &=  ~PARENB;            // Бит четности выключен
    tty.c_cflag     &=  ~CSTOPB;            // Стоп бит один
    tty.c_cflag     &=  ~CSIZE;             // Очистить маску размера сообщения
    tty.c_cflag     |=  CS8;                // Сообщение 8 бит

    tty.c_cflag     &=  ~CRTSCTS;           // Отключить аппаратное управление потоком RTS / CTS
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
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
    char response[1024];
    memset(response, '\0', sizeof(response));
    char symbol = '\0';
    do {
        n = read(fd, &symbol, 1);
        sprintf(&response[num], "%c", symbol);
        num += n;
    } while( symbol != '\r' && n > 0);

    if (strlen(response)>0)
    {
        logg.err(response);
    }

    
}

void Uart::uart_transmit(int fd, char* str,size_t size)
{
    if (write(fd,str,size) == -1)
    {
        logg.err("Write error!");
    }

}

