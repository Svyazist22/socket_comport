/**
  @brief   Объектно-ориентированная обертка над pipe
  @details Позволяет вызвать метод класса в потоке 1, переданного в init, если в потоке2 прозошла запись в pipe
  @author  Nikita Shishkov
  @date    2021-05-04
  */

#pragma once

#include "eventmonitor.h"

#include <unistd.h>

class Pipe
{
    /// Объект, мониторящий события в pipe
    EventMonitor em;
    /// Файловые дескрипторы pipe
    int pipefds[2];

public:
    /**
     * @brief Деструктор
     */
    ~Pipe ()
    {
        // Закрываем pipe
        ::close (pipefds[0]);
        ::close (pipefds[1]);
    }

    /**
     * @brief init Назначить метод класса _object, который будет вызван при событии записи в pipe
     * @param _object Указатель на объект
     * @param T::*_methodPointer Указатель на метод
     * @return true в случае успеха
     */
    template<class T>
    inline bool init (T* _object, void (T::*_methodPointer)())
    {
        bool ok = pipe (pipefds) == 0;
        em.onREvent = method (_object, _methodPointer);
        em.start (pipefds[0]);
        return ok;
    }

    /**
     * @brief read Прочитать из pipe
     * @param p Указатель, по которому необходимо записать данные
     * @param size Максимальный размер данных, которые могут быть вычитаны за раз
     * @return Размер принятых данных
     */
    inline size_t read (void* p, size_t size)
    {
        return ::read (pipefds[0], p, size);
    }

    /**
     * @brief write Записать данные в pipe
     * @param p Указатель на записываемые данные
     * @param size Размер записываемых данных
     * @return Размер записанных данных
     */
    inline size_t write (const void* p, size_t size)
    {
        return ::write (pipefds[1], p, size);
    }
};
