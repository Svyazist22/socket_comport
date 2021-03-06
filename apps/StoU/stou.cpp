/**
 * @file stou.cpp
 * @author Vladislav Bakanov
 * @brief Слушает сокет, отправляет полученные данные на UART
 * @version 0.1
 * @date 2021-07-06
 */

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include "../../include/app/stou.h"

int main (int argc, char const *argv[])
{
    Server serv;
    Uart uart;
    Logger log;

    char* buf = new char;

    int fd = uart.uartFd();
    if ( fd < 0 )
    {
        return 0;
    }

    //Инициализация сервера (АРМ)
    serv.serv_init(); 

    //Инициализация компорта
    uart.uartInit (fd); 

    while(1)
    {
        buf = serv.serv_read();                     // Получаем сообщение от клиента
        
        // Команда остановки программ
        if (strcmp (buf, "stop") == 0)
        {   
            delete [] buf;
            serv.serv_stop();
            log.err ("The program is stopped!");
            return 0;
        }

        if (strlen (buf) > 0)
        {
            log.info ("Message received:%s",buf); 
            uart.uartTransmit (fd, buf, strlen (buf)); //Отправляем на компорт
        }
    }
    
    return 0;
}
