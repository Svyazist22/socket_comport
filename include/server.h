/**
 * @file server.h
 * @author Vlad Bakanov
 * @brief Socket server
 * @version 0.1
 * @date 2021-06-29
 */

#include "logger.h"

#pragma once

class Server
{
    /// Socket descriptor
    static int sd;
    /// Код возврата связывания сокета с адрессом
    static int bind_serv;
    /// Код возврата прослушивания сокета
    static int listen_serv;
    /// Новый socket descriptor для общения с client
    static int accept_serv;

public:

    /**
     * @brief Инициализация server
     */
    static void serv_init();

    /**
     * @brief Прослушивание client. Отправляет echo клиенту
     */
    static void serv_read();
};



