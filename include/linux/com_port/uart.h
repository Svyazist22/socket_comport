/**
 * @file uart.h
 * @author Vladislav Bakanov
 * @brief Работа с USB<=>UART
 * @version 0.1
 * @date 2021-06-28
 */

#include "../../cross_platform/fifo.h"
#include "../additional/logger.h"
#include "../../cross_platform/crypt.h"

#pragma once

class Uart
{

    int setResult;          // Результат сохранения настроек UART

public:

    /**
     * @brief Ошибки uart
     */
    enum errorUart
    {
        errNo,             // Ошибок нет
        errInit            // Ошибка сохраннения настроек
    };
   
    /**
     * @brief Инициализация USB<=>UART
     * @param [in] fd       file descriptor
     */
    void uartInit(int fd);

    /**
     * @brief Прослушивание UART и запись в fifo
     * @param [in] fd       file descriptor 
     * @param [in,out] buf  Указаель на массив для записи полученных данных
     */
    void uartReceive(int fd,fifo_t *buf);

    /**
     * @brief Отправка данных на UART
     * @param [in] fd       file descriptor 
     * @param [in] str      Указатель на отправляемое сообщение
     * @param [in] size     Размер сообщения
     */
    void uartTransmit(int fd, char* str,size_t size);

    /**
     * @brief Устанавливает связь между файлом компорта и файловым дискриптером
     * @return file descriptor  
     */
    int uartFd();

    /**
     * @brief   Возвращает код ошибки
     * @return  Возвращает элемент enum в соответсвии с тем, какая ошибка произошла или её не было
     */
    Uart::errorUart getError();
 
};



