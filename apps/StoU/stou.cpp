/**
 * @file stou.cpp
 * @author Vladislav Bakanov
 * @brief Слушает сокет, отправляет полученные данные на UART
 * @version 0.1
 * @date 2021-07-06
 */

#include "../../include/stou.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    Server serv;
    Uart uart;
    Logger log;

    char* buf = new char;

    int fd = open("/dev/ttyUSB0",O_RDWR);
    if ( fd < 0 )
    {
        log.err("Open /dev/ttyUSB0 ERROR: %s",strerror(errno));
        return 0;
    }

    //Инициализация сервера (АРМ)
    serv.serv_init(); 

    //Инициализация компорта
    uart.uart_init(fd); 

    while(1)
    {
        buf = serv.serv_read();     // Получаем сообщение от клиента
        
        // Команда остановки программ
        if (strcmp(buf,"stop")==0)
        {   
            delete [] buf;
            serv.serv_stop();
            log.err("The program is stopped!");
            return 0;
        }

        if (strlen(buf)>0)
        {
            log.info("Message received:%s",buf); 
            uart.uart_transmit(fd,buf,strlen(buf)); //Отправляем на компорт
        }
    }
    
    return 0;
}
