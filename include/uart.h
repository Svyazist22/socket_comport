/**
 * @file uart.h
 * @author Vladislav Bakanov
 * @brief Работа с USB<=>UART
 * @version 0.1
 * @date 2021-06-28
 */

#include "fifo.h"
#include "logger.h"

#pragma once

/**
 * @brief Инициализация USB<=>UART
 * @param fd file descriptor ("/dev/ttyUSB0")
 */
void uart_init(int fd);

/**
 * @brief Прослушивание UART и запись в fifo
 * @param fd file descriptor ("/dev/ttyUSB0")
 * @param buf Массив для записи полученных данных
 * @warning Сообщение должно заканчиваться спец. символом "\a"
 */
void uart_receive(int fd,fifo_t buf);
