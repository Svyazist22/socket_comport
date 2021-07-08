/**
 * @file client.h
 * @author Vladislav Bakanov
 * @brief Socket client
 * @version 0.1
 * @date 2021-06-29
 */

#include "logger.h"

#pragma once

class Client
{
    /// Socket descriptor
    static int sd;
    /// Код возврата связывания сокета с server
    static int connect_client;

public:

    /**
     * @brief Инициализация client
     */
    static void client_init();

    /**
     * @brief Отправляет данные client
     * @param str Указатель на сообщение
     */
    static void client_write(char* str);
};
