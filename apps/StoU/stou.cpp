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

    int fd = open("/dev/ttyUSB0",O_RDWR| O_NONBLOCK | O_NDELAY );
    if ( fd < 0 )
    {
        log.err("Open /dev/ttyUSB0 ERROR");
        return 0;
    }

    //Инициализация сервера (АРМ)
    serv.serv_init(); 

    //Инициализация компорта
    uart.uart_init(fd); 

   
        // Получаем сообщение от клиента (софт)
        buf = serv.serv_read(); 
        log.info("Message received:%s",buf); 
        
        //Отправляем на компорт
        uart.uart_transmit(fd,buf,strlen(buf)); 
    
    
    

    return 0;
}
