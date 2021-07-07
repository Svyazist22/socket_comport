/**
 * @file utos.cpp
 * @author Vladislav Bakanov
 * @brief Слушает UART, проверяет полученную информацию и отправляет на соект результат
 * @version 0.1
 * @date 2021-07-06
 * @warning ШАБЛОН. Сейчас работает как client.
 */

#include "../../include/utos.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    Logger log;

    int fd = open("/dev/ttyUSB0",O_RDWR);
    if ( fd < 0 )
    {
        log.err("Open /dev/ttyUSB0 ERROR");
        return 0;
    }
    fifo_t fbuf;
    char *str = new char; // Команда с консоли
    char *buf = new char; // Сообщение с компорта
    Client cl;
    Uart uart;

    cl.client_init();
    uart.uart_init(fd);
    
    while (1)
    {
        // Вводим команду и отправляем на компорт
        printf("Write command:");
        std::cin.getline(str,32);
        cl.client_write(str);

        sleep(1); // Т.к. используется один компорт нужна задержка

        // Слушаем компорт
        uart.uart_receive(fd,fbuf);

        log.warn("%s - %d",str,strlen(str));
        log.warn("%s - %d",buf,strlen(buf));
    }
    
 



    return 0;
}
