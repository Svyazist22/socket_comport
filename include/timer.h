/**
  @brief Timer
  @details Класс пренадназначен для регулярного вызова методов классов.
  Основан на системе событий Poll
  Для создания таймера необходимо определить метод, который будет вызван,
  когда таймер сработает: onTick = method ([object], [method's pointer]);
  Например:
  timer.onTick = method (this, &MyClass::onTimer);
  И затем необходимо вызвать метод start с указанием числа милисекунд периода таймера
  Метод stop останавливает таймер, is Enabled сообщает о том, работает ли таймер в настоящий момент

  Функциональность таймера сохранятеся только если в главном цикле сообщений потока
  работает статическая функция EventMonitor::dispatch.

  @author Nikita Shishkov
  @date 2021-04-30
  */

#pragma once

#include "eventmonitor.h"

class Timer
{
    /// Тип для события onTick
    typedef Method<void> OnTick;

    /// Файловый дескриптор
    int fd = -1;

    /// Монитор событий
    EventMonitor eventMonitor;

    /// Обработчик события onTick
    void onEvent ();

public:
    /// Событие onTick
    OnTick onTick;

    /// Деструктор
    ~Timer() { stop(); }

    /**
     * @brief start Запустить таймер
     * @param msec Задержка, мс
     * @return true в случае успеха
     */
    bool start (unsigned msec);

    /**
     * @brief stop Остановить таймер
     */
    void stop ();

    /**
     * @brief isEnabled Проверить, работает ли таймер
     * @return true, если работает
     */
    inline bool isEnabled () { return fd != -1; }
};
