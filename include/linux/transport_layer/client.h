/**
 * @file client.h
 * @author Vladislav Bakanov
 * @brief Socket client
 * @version 0.1
 * @date 2021-06-29
 */

#include "../additional/logger.h"

#pragma once

class Client
{
    /// Socket descriptor
    int sd;

    /// Код возврата связывания сокета с server
    int connectClient;

public:

    /**
     * @brief   Ошибки клиентского приложения
     */
    enum errorClient
    {
        errNo,         // Ошибок нет
        errSd,         // Ошибка создание сокет-дискриптора
        errConn,       // Ошибка установки связи с сервером 
    };

    /**
     * @brief   Инициализация client
     */
    void clientInit();

    /**
     * @brief   Отправляет данные client
     * @param [in] str Указатель на сообщение
     */
    void clientWrite (char* str);

    /**
     * @brief   Останавливает соединение с сервером
     */
    void clientStop();

    /**
     * @brief   Возвращает код ошибки
     * @return  Возвращает элемент enum в соответсвии с тем, какая ошибка произошла или её не было
     */
    errorClient getError();
};
