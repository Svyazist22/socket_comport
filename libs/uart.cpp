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

    struct termios term;
    
    tcgetattr(fd,&term);        // Получение стандартных настроек

    cfsetspeed(&term, B9600);   // Скорость приема и передачи

    term.c_cflag &= ~PARENB;    // Бит четности выключен
    term.c_cflag &= ~CSTOPB;    // Стоп бит один
    
    term.c_cflag &= ~CSIZE;     // Очистить маску размера сообщения
    term.c_cflag |=  CS8;       // Сообщение 8 бит

    term.c_cflag &= ~CRTSCTS;   // Отключить аппаратное управление потоком RTS / CTS

    term.c_lflag &= ~ICANON;    // Неканонический режим

    term.c_lflag &= ~ECHO;      // Выключить echo
    term.c_lflag &= ~ECHOE;     // Выключить  erasure
    term.c_lflag &= ~ECHONL;    // Выключить new-line echo

    term.c_lflag &= ~ISIG;      // Отключить интерпретацию INTR, QUIT и SUSP

    term.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

    term.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Отключаем любую специальную обработку полученных байтов

    term.c_oflag &= ~OPOST; // Предотвращаем специальную интерпретацию выходных байтов (например, символы новой строки)
    term.c_oflag &= ~ONLCR; // Запретить преобразование новой строки в возврат каретки / перевод строки
    
    term.c_cc[VMIN] = 40;       // Ожидание 40 символов
    term.c_cc[VTIME] = 30;      // Ожидание 3 секунды

    if (tcsetattr(fd, TCSANOW, &term) < 0)
    {
        logg.err("Unable to set port parameters");     
    }
    
}


void Uart::uart_receive(int fd, fifo_t buf)
{
    char read_buffer[10];
    char str[64];
    int bytes_read;
    int i = 0;
    bool rw;
    memset(str,0,sizeof(str)); 

    while (1)
    {
        bytes_read = read(fd,&read_buffer,1); 
        if (read_buffer[0] != '\a') 
        {
            str[i]=read_buffer[0];
            i++;
        }
        else
        {
            logg.info("%s", str);
            if (fifo_free(&buf)>63) // Запись в буффер
            {
                fifo_write_push(&buf,&str,64);
            }
            else
            {
                logg.warn("The buffer is full");
            }
            memset(str,0,sizeof(str));
            i=0;
        }  
    }
}

void Uart::uart_transmit(int fd, char* str,size_t size)
{
    if (write(fd,str,size) == -1)
    {
        logg.err("Write error!");
    }

}

