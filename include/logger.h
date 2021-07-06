/**
  @brief Logger
  @author Nikita Shishkov
  @date 2021-05-05
  */

#pragma once

#include <stdarg.h>
#include <string.h>
#include <time.h>


/**
 * Уровень вывода сообщений
 */
typedef enum
{
    llNone = 0,
    llError = 1,
    llWarning = 2,
    llInfo = 3,
    llDebug = 4,
} log_level_t;

class Logger
{
    static struct timespec time_start;
    void printf (log_level_t log_level, const char* str, va_list arglist);

protected:
    log_level_t init_log_level = llInfo;
    char modName[9] = "  GLOBAL";

public:
    /// Конструктор лога
    Logger ();

    /**
     * @brief dbg Вывести в лог сообщение об ошибке
     * @param str строка, описывающая форматированный вывод
     */
    void err  (const char* str, ...);

    /**
     * @brief dbg Вывести в лог предупреждение
     * @param str строка, описывающая форматированный вывод
     */
    void warn (const char* str, ...);

    /**
     * @brief dbg Вывести в лог информацию для пользователя
     * @param str строка, описывающая форматированный вывод
     */
    void info (const char* str, ...);

    /**
     * @brief dbg Вывести в лог отладочное сообщение
     * @param str строка, описывающая форматированный вывод
     */
    void dbg  (const char* str, ...);

    /**
     * @brief lastMsg Получить указатель на буфер, содержайщий текст последней ошибки
     * @return указатель на буфер
     */
    const char* lastMsg ();

};


class LoggerModule : public Logger
{
public:
    LoggerModule (const char* moduleName, log_level_t init_log_level)
    {
        size_t s = strlen (moduleName);
        if (s > sizeof (modName) - 1)
            s = sizeof (modName) - 1;
        size_t padding = sizeof (modName) - 1 - s;
        memset (modName, ' ', padding);
        memcpy (modName + padding, moduleName, s);
        this->init_log_level = init_log_level;
    }
};
