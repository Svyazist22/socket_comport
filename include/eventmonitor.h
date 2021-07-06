/**
  @brief Event Monitor monitors the selected events using system POLL
  @details Простейший монитор событий.
  @author Nikita Shishkov
  @date 2021-05-05

  @todo Сделать так, чтобы EventMonitor мог работать в каждом потоке свой
  */

#pragma once

#include <stddef.h>

#include "method.h"

class EventMonitor
{
    typedef Method<void> Event;

    /// Начало двусвязного списка наблюдателей
    static EventMonitor* first;
    /// Конец двусвязного списка наблюдателей
    static EventMonitor* last;
    /// Флаг необходимости выхода из dispatch
    static bool terminateFlag;
    /// Флаг необходимости переформировать список наблюдаемых дескрипторов
    static bool changed;
    /// Код возврата из dispatch
    static int returnCode;
    /// Суммарное число наблюдаемых файловых дескрипторов
    static size_t count;

    /// Файловый дескриптор, по которому ведётся наблюдение за событиями
    int fd = -1;    
    /// Наблюдать за событиями чтения
    bool read;
    /// Наблюдать за событиями записи
    bool write;
    /// Указатель на предыдущий объект наблюдения событий
    EventMonitor* next = nullptr;
    /// Указатель на следующий объект наблюдения событий
    EventMonitor* prev = nullptr;

public:
    /// Событие чтения
    Event onREvent;

    /// Событие записи
    Event onWEvent;

    /// Деструктор
    ~EventMonitor ();

    /**
     * @brief start Запустить процесс наблюдения за событиями
     * @param fd Номер дескриптора для наблюдения
     * @param read если true, то необходимо наблюдать события чтения
     * @param write если true, то необходимо наблюдать события записи
     */
    void start (int fd, bool read = true, bool write = false);

    /**
     * @brief stop Остановить процесс наблюдения за событиями
     */
    void stop ();

    /**
     * @brief dispatch Ожидать прихода событий
     * @param timeout_ms Максимальное время ожидания событий
     * @return Код возврата из метода quit (code)
     */
    static int dispatch (unsigned timeout_ms = 100);

    /**
     * @brief quit Выйти из функции dispatch
     * @param returnCode
     */
    static void quit (int returnCode = 0);
};
