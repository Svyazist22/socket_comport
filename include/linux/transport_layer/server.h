/**
 * @file server.h
 * @author Vladislav Bakanov
 * @brief Socket server
 * @version 0.1
 * @date 2021-06-29
 */

#include "../additional/logger.h"

#pragma once

#define PORT 64300

class Server
{
    /// Socket descriptor
    int sd;
    /// Код возврата связывания сокета с адрессом
    int bind_serv;
    /// Код возврата прослушивания сокета
    int listen_serv;
    /// Новый socket descriptor для общения с client
    int accept_serv;

public:

    /**
     * @brief   Инициализация server
     */
    void serv_init();

    /**
     * @brief   Прослушивание client
     * @return  Возвращает указатель на полученное сообщение
     */
    char* serv_read();

    /**
     * @brief   Останавливает соединение
     */
    void serv_stop();
    
};



