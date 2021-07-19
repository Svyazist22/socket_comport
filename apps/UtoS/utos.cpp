/**
 * @file utos.cpp
 * @author Vladislav Bakanov
 * @brief Слушает UART, проверяет полученную информацию
 * @version 0.1
 * @date 2021-07-06
 */

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

#include "../../include/utos.h"

int main(int argc, char const *argv[])
{
    Logger log;
    Client cl;
    Uart uart;

    char command;                                   // Команда с консоли при ошибке
    int err;                                        // Код возврата ошибки
  
    fifo_t fifo_s;                                  // Объект fifo
    uint8_t *fifo_buf = new uint8_t;                // Указатель на массив fifo   
    
    char *str_cons = new char[1024];                // Команда с консоли
    char *str_com = new char[1024];                 // Сообщение с компорта
    char *h1 = new char[17];                        // Хэш отправленного сообщения
    char *h2 = new char[17];                        // Хэш полученного сообщения

    // Инициализация клиента
    while (1)
    {
        err = cl.client_init();
        if((err == cl.err_conn) || (err == cl.err_sd))
        {
            printf("You can (r)epeat or (c)lose programm:");
            std::cin >> command;  
            switch (command)
            {
            case 'r':               // Повторить инициализацию                      
                break;
            case 'c':               // Закрыть программу          
                return 0;
            default:                // Закрыть программу при неверном символе
                return 0;
            }
        }
        else
        {
            break;                  // Выйти из цикла инициализации, если ошибок нет
        }
    }

    // Дискриптор компорта 
    int fd = uart.uart_fd();                        

    // Инициализация компорта
    while (1)
    {
        err = uart.uart_init(fd);
        if( err == uart.err_init)
        {
            printf("You can (r)epeat or (c)lose programm:");
            std::cin >> command;
            switch (command)
            {
            case 'r':               // Повторить инициализацию                
                break;
            case 'c':               // Закрыть программу                  
                return 0;
            default:                // Закрыть программу при неверном символе
                return 0;
            }
        }
        else
        {
            break;                  // Выйти из цикла инициализации, если ошибок нет
        }
    }

    // Инициализация fifo-буффера
    fifo_init(&fifo_s,fifo_buf,10240);                
  
    while (1)
    {
        printf("Write command:");
        std::cin.getline(str_cons,64);                      
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
     
        uart.uart_receive(fd,&fifo_s);              // Слушаем компорт и записываем в буффер
        fifo_read_pop(&fifo_s,str_com,1024);        // Берем из буффера полученное сообщение
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
