/**
  @brief Wrapper on astring function
  @details Стандартная С-шная строка, форматирующаяся посредством asprintf, но только с деструктором
  @author Nikita Shishkov
  @date 2021-05-08
  */

#pragma once

#include <stddef.h>

class AString
{
    /// Указатель на строку
    char* p = nullptr;
    /// Длина строки
    size_t len = 0;
public:
    /// Конструктор
    AString () {}

    /// Деструктор
    ~AString ();

    /// Конструктор копирования удалён
    AString (AString& other) = delete;

    /// Оператор копирования удалён
    AString& operator = (AString& other) = delete;

    /// Форматированный вывод аналогично asprintf
    bool printf(const char* fmt, ...);

    /**
     * @brief assign Скопировать из С-строки
     * @param str Копируемая строка
     * @return true, если память выделена
     */
    bool assign(const char* str); //! assign()

    /// Получить указатель на строку в С-стиле
    char* data (void) { return p; }

    /// Получить const указатель на строку в С-стиле
    const char* c_str (void) { return p ? p : ""; }

    /// Получить размер строки в байтах
    size_t size (void) { return len; }

    /// Доступ к элементам строки без проверки на границы
    char operator [] (size_t index) { return p[index]; } //! char& operator [] (size_t index)

    /// Доступ к неизменяемым элементам строки без проверки на границы
    const char operator [] (size_t index) const { return p[index]; } //! char operator[] (size_t index) const
};

