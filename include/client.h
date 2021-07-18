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
    int sd;
    /// Код возврата связывания сокета с server
    int connect_client;

public:

    /**
     * @brief Ошибки клиентского приложения
     */
    enum error_client
    {
        err_no,         // Ошибок нет
        err_sd,         // Ошибка создание сокет-дискриптора
        err_conn,       // Ошибка установки связи с сервером 

    };

    /**
     * @brief Инициализация client
     * @return Возвращает элемент enum в соответсвии с тем, какая ошибка произошла или её не было
     */
    error_client client_init();

    /**
     * @brief Отправляет данные client
     * @param str Указатель на сообщение
     */
    void client_write(char* str);

    /**
     * @brief Останавливает соединение
     */
    void client_stop();
};
