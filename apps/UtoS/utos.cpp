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

    int fd = uart.uart_fd();
    if (fd < 0 )
    {
        return 0;
    }
    

    fifo_t fbuf; 
    uint8_t *fifo_buf = new uint8_t;
    
    char *str_cons = new char[1024];                // Команда с консоли
    char *str_com = new char[1024];                 // Сообщение с компорта
    char *h1 = new char[17];                        // Хэш отправленного сообщения
    char *h2 = new char[17];                        // Хэш полученного сообщения

    cl.client_init();                               // Инициализация клианта
    uart.uart_init(fd);                             // Инициализация компорта
    fifo_init(&fbuf,fifo_buf,10240);                // Инициализация fifo-буффера
  
    while (1)
    {
        printf("Write command:");
        //std::cin.getline(str_cons,1024);            // Считываем команду с консоли
        std::cin >> str_cons;
        // Проверка на непустое сообщение
        if (strlen(str_cons)==0)
        {
            log.err("Command input error (the command is empty). Please try again");
            continue;
        }
        
        cl.client_write(str_cons);                  // Отправляем на компорт
        create_hash(str_cons,strlen(str_cons),h1);  // Получаем хэш отправленного сообщения

        // Команда остановки программ
        if (strcmp(str_cons,"stop")==0)
        {
            cl.client_stop();
            log.err("The program is stopped!");
            return 0;
        }
        
        sleep(1);                                   // Т.к. используется один компорт нужна задержка
     
        uart.uart_receive(fd,&fbuf);                // Слушаем компорт и записываем в буффер
        fifo_read_pop(&fbuf,str_com,1024);          // Берем из буффера полученное сообщение
        log.info("Message received from UART:%s",str_com);
        create_hash(str_com,strlen(str_com),h2);    // Получаем хэш полученного сообщения
        
        // Сравниваем хэши сообщений
        if(compare_hash(h1,h2))         
        {
            log.info("The messages are the same");
        }
        else
        {
            log.err("The messages are different!");
        }
    }
    
    return 0;
}
