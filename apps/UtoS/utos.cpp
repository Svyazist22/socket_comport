/**
 * @file utos.cpp
 * @author Vladislav Bakanov
 * @brief Слушает UART, проверяет полученную информацию и отправляет на сокет результат
 * @version 0.1
 * @date 2021-07-06
 */

#include "../../include/utos.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    Logger log;
    Client cl;
    Uart uart;

    int fd = open("/dev/ttyUSB0",O_RDWR);
    if ( fd < 0 )
    {
        log.err("Open /dev/ttyUSB0 ERROR");
        return 0;
    }

    fifo_t fbuf; 
    uint8_t *fifo_buf = new uint8_t;
    
    char hash[16]; 
    char *str = new char; // Команда с консоли
    char *buf = new char; // Сообщение с компорта

    cl.client_init();
    uart.uart_init(fd);
    fifo_init(&fbuf,fifo_buf,10240);
    
    while (1)
    {
        printf("Write command:");
        std::cin.getline(str,32);   // Считываем команду с консоли
        cl.client_write(str);       // Отправляем на компорт

        sleep(1); // Т.к. используется один компорт нужна задержка

        uart.uart_receive(fd,&fbuf); // Слушаем компорт
        fifo_read_pop(&fbuf,buf,1024);
        log.info("Message received from UART:%s",str);
        

        

        //get_hash(str,strlen(str),hash); 
        //log.info("Hash:%s",hash);
    }
    
    return 0;
}
