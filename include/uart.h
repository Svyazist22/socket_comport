/**
 * @file uart.h
 * @author Vladislav Bakanov
 * @brief Работа с USB<=>UART
 * @version 0.1
 * @date 2021-06-28
 */

#include "fifo.h"
#include "logger.h"
#include "crypt.h"

#pragma once

class Uart
{

public:

    /**
     * @brief Инициализация USB<=>UART
     * @param [in] fd       file descriptor ("/dev/ttyUSB0")
     */
    void uart_init(int fd);

    /**
     * @brief Прослушивание UART и запись в fifo
     * @param [in] fd       file descriptor ("/dev/ttyUSB0")
     * @param [in,out] buf  Указаель на массив для записи полученных данных
     */
    void uart_receive(int fd,fifo_t *buf);

    /**
     * @brief Отправка данных на UART
     * @param [in] fd       file descriptor ("/dev/ttyUSB0")
     * @param [in] str      Указатель на отправляемое сообщение
     * @param [in] size     Размер сообщения
     */
    void uart_transmit(int fd, char* str,size_t size);
 
};



