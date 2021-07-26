/**
 * @file uart.cpp
 * @author Vlad Bakanov
 * @brief Работа с USB<=>UART
 * @version 0.1
 * @date 2021-06-28
 */

#include <fcntl.h>
#include <termios.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>

#include "../../../include/linux/com_port/uart.h"

Logger logg;

void Uart::uartInit (int fd){

    struct termios tty;
    
    tcgetattr (fd, &tty);                   // Получение стандартных настроек

    cfsetspeed (&tty, B9600);               // Скорость приема и передачи

    tty.c_cflag     &=  ~PARENB;            // Бит четности выключен       
    tty.c_cflag     &=  ~CSTOPB;            // Стоп бит один
    tty.c_cflag     &=  ~CSIZE;             // Очистить маску
    tty.c_cflag     |=  CS8;                // Сообщение 8 бит
    tty.c_cflag     &=  ~CRTSCTS;           // Управление потоком RTS/CTS выключено 
    tty.c_cc[VMIN]   =  0;                  // Ожидание 0 бит  
    tty.c_cc[VTIME]  =  0;                  // Ожидание 0 дсек  
    tty.c_cflag     |=  CREAD | CLOCAL;     // CLOCAL позволяет считывать данные.  

    tty.c_lflag &= ~ICANON;                 // Канонический режим выключен
    tty.c_lflag &= ~(ECHO|ECHOE|ECHONL);    // Отключить ЭХО
    tty.c_lflag &= ~ISIG;                   // Отключить сигнальные символы
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Отключение программного управления потоком
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Отключение специальной 
                                                                     // обработки байтов при приеме
    tty.c_oflag &= ~OPOST;                  // Отключить преобразование выходных символов
    tty.c_oflag &= ~ONLCR;                  // Запретить преобразование новой строки в возврат каретки

    cfmakeraw (&tty);
    setResult = tcsetattr (fd, TCSANOW, &tty);
}

void Uart::uartReceive (int fd, fifo_t *buf)
{
    char response[1024];
    memset(response, '\0', sizeof (response));

    //fcntl(fd, F_SETFL, FNDELAY);
    read(fd, response, 1024);

    //tcflush( fd, TCIFLUSH );

    if (fifo_free (buf) >= 1024)
    {
        fifo_write_push (buf, response, 1024);        
    }
    else
    {
        logg.err ("The buffer is ful");
    }
}

void Uart::uartTransmit (int fd, char* str, size_t size)
{
    if (write (fd, str, size) == -1)
    {
        logg.err ("Write error! %s", strerror (errno));
    }
    else
    {
        logg.info ("The message was sent to the UART");
    }
}

int Uart::uartFd ()
{
    int fd = -1;
    char addr[64] = "/dev/ttyUSB0"; // Стандартный путь до компорта
    char command;                   // Команда с консоли при ошибке

    while (1)
    {
        fd = open (addr,O_RDWR);
        if (fd > 0)
        {
            break;
        }
        else
        {
            logg.err ("Open ERROR %s: %s", addr, strerror (errno));
            printf ("You can (r)epeat, (w)rite new addres, (c)lose programm:");
            command = '\0';
            std::cin >> command;    // Ввод выбора действия при ошибке открытия
            command = (char) tolower (command);   
            switch (command)
            {
            case 'r':               // Повторить открытие                  
                break;

            case 'w':               // Ввести новый адрес                     
            printf ("Addres (ex.:/dev/ttyUSB0):");
            std::cin >> addr;
                break;

            case 'c':               // Завершить программу
                exit (EXIT_FAILURE);
                break;

            default:                // Неверная команда завершает программу    
                exit (EXIT_FAILURE);
                break;
            }
        }
    }
    return fd;
}

Uart::errorUart Uart::getError ()
{
    if (setResult < 0)
    {
        return errInit;     
    }
    else
    {
        return errNo;
    }
}