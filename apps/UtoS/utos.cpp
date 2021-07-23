/**
 * @file utos.cpp
 * @author Vladislav Bakanov
 * @brief Слушает UART, проверяет полученную информацию
 * @version 0.2
 * @date 2021-07-06
 */

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <ctime>

#include "../../include/app/utos.h"

int main(int argc, char const *argv[])
{
    Logger log;
    Client cl;
    Uart uart; 
    char command;                                   // Команда с консоли при ошибке
    int err;                                        // Код возврата ошибки
    bool running = true;                            // Условие работы программы

    clock_t start_time = 0;
    clock_t end_time = 0;
  
    fifo_t fifo_s;                                  // Объект fifo
    uint8_t *fifo_buf = new uint8_t;                // Указатель на массив fifo   

    char *str_cons = new char[1024];                // Команда с консоли
    char *str_com = new char[1024];                 // Сообщение с компорта
    char *h1 = new char[17];                        // Хэш отправленного сообщения
    char *h2 = new char[17];                        // Хэш полученного сообщения

    // Инициализация клиента
    cl.clientInit ();
    
    // Проверка инициализации
    while (cl.getError() != cl.errNo)
    {
        err = cl.getError();    // Получаем код ошибки

        if(err == cl.errSd)
        {
            log.err("Error create socket: %s",strerror (errno));
        }
        else if(err == cl.errConn)
        {
            log.err("Error create connect: %s",strerror (errno));
        }

        std::cout << "You can (r)epeat or (c)lose programm:";
        std::cin >> command;
        command = (char) tolower (command); 
        switch (command)
        {
        case 'r':               // Повторить инициализацию 
            cl.clientInit();                   
            break;
        case 'c':               // Завершить программу
            delete str_com;
            delete str_cons;
            delete h1;
            delete h2;
            cl.clientStop();
            return 0;
        default:                // Завершить программу при неверном символе
            delete str_com;
            delete str_cons;
            delete h1;
            delete h2;
            cl.clientStop();
            return 0;
        }
    }

    // Дискриптор ком-порта 
    int fd = uart.uartFd();                        

    // Инициализация компорта
    uart.uartInit (fd);

    // Проверка инициализации
    while (uart.getError() != uart.errNo)
    {
        if(uart.getError() == uart.errInit)
        {
            log.err("Unable to set port parameters: %s", strerror (errno));
            std::cout<<"You can (r)epeat or (c)lose programm:";
            std::cin >> command;
            command = (char) tolower( command); 
            switch (command)
            {
            case 'r':               // Повторить инициализацию
            uart.uartInit (fd);              
                break;
            case 'c':               // Завершить программу                  
                delete str_com;
                delete str_cons;
                delete h1;
                delete h2;
                cl.clientStop();
                return 0;
            default:                // Завершить программу при неверном символе
                delete str_com;
                delete str_cons;
                delete h1;
                delete h2;
                cl.clientStop();
                return 0;
            }
        }
    }

    // Инициализация fifo-буффера
    fifo_init (&fifo_s, fifo_buf, 10240);                
  
    while (running)
    {
        printf ("Write command:");
        std::cin.getline (str_cons,64);                      
        // Проверка на непустое сообщение
        if (strlen (str_cons) == 0)
        {
            log.err("Command input error (the command is empty). Please try again");
            continue;
        }
        
        start_time = clock();
        
        cl.clientWrite (str_cons);                      // Отправляем на компорт
        create_hash (str_cons, strlen (str_cons), h1);  // Получаем хэш отправленного сообщения
    
        // Команда остановки программ
        if (strcmp (str_cons, "stop") == 0)
        {
            log.err ("The program is stopped!");
            running = false;
            break;
        }
        
        sleep(1);                                       // Т.к. используется один компорт нужна задержка
     
        uart.uartReceive (fd, &fifo_s);                 // Слушаем компорт и записываем в буффер
        end_time = clock();
        fifo_read_pop (&fifo_s, str_com, 1024);          // Берем из буффера полученное сообщение
        log.info ("Message received from UART:%s", str_com);
        create_hash (str_com, strlen (str_com), h2);    // Получаем хэш полученного сообщения
        
        // Сравниваем хэши сообщений
        if(compare_hash (h1, h2))         
        {
            log.info ("The messages are the same");
        }
        else
        {
            log.err ("The messages are different!");
        }

        log.info ("Time: %lf ms", (double) (end_time - start_time) * 1000.0 / CLOCKS_PER_SEC);
    }

    //delete [] str_cons;
    //delete [] str_com;
    delete [] h1;
    delete [] h2;
    delete [] fifo_buf;
    cl.clientStop();
    return 0;
}
