/**
 * @file stou.cpp
 * @author Vladislav Bakanov
 * @brief Слушает сокет, отправляет полученные данные на UART
 * @version 0.1
 * @date 2021-07-06
 * @warning ШАБЛОН. Сейчас работает как server
 */

#include "../../include/stou.h"

#include <iostream>
#include <fcntl.h>

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

    serv.serv_init();
    uart.uart_init(fd);
    buf = serv.serv_read();
    log.info("Message received:%s",buf);
    uart.uart_transmit(fd,buf,strlen(buf));





    


    
    return 0;
}
