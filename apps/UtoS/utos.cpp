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
#include <thread>

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
    
    char *str = new char; // Команда с консоли
    char *buf = new char; // Сообщение с компорта
    char *h1 = new char[16];  // Хэш отправленного сообщения
    char *h2 = new char[16]; // Хэш полученного сообщения

    cl.client_init();
    uart.uart_init(fd);
    fifo_init(&fbuf,fifo_buf,10240);
  
    while (1)
    {
        printf("Write command:");
        std::cin.getline(str,1024);       // Считываем команду с консоли
        cl.client_write(str);           // Отправляем на компорт
        get_hash(str,strlen(str),h1);   // Получаем хэш отправленного сообщения
        log.info("%s=%d",str,strlen(str));
       

        sleep(1);                       // Т.к. используется один компорт нужна задержка
     

        uart.uart_receive(fd,&fbuf);    // Слушаем компорт и записываем в буффер
        fifo_read_pop(&fbuf,buf,1024);  // Берем из буффера полученное сообщение
        log.info("Message received from UART:%s",buf);
        get_hash(buf,strlen(buf),h2);   // Получаем хэш полученного сообщения
        log.info("%s=%d",buf,strlen(buf));
        
        // Сравниваем хэши сообщений
        if(compare_hash(h1,h2))         
        {
            log.info("The messages are the same");
        }
        else
        {
            log.err("The messages are different!");
        }
      
        log.info("%s",h2);


    }
    
    return 0;
}
