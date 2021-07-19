#include "../../include/cross_platform/fifo.h"

#include <string.h>

#ifdef __AVR__
#include <util/atomic.h>
#endif

#include "../../include/linux/additional/minmax.h"

// Инициализация FIFO
void fifo_init (fifo_t* fifo, uint8_t *buff_ptr, uint_fifo_t buff_size)
{
    fifo->rd = fifo->wr = 0;
    fifo->size = buff_size;
    fifo->buff = buff_ptr;
}

// сброс FIFO
void fifo_clear (fifo_t* fifo)
{
#ifdef __AVR__
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
#endif
    {
        fifo->rd = fifo->wr = 0;
    }
}

// Свободное пространство в FIFO буфере
uint_fifo_t fifo_free(fifo_t* fifo)
{
#ifdef __AVR__
    uint_fifo_t rd;
    uint_fifo_t wr;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
    {
        rd = fifo->rd;
        wr = fifo->wr;
    }
    return (rd - wr - 1) & (fifo->size - 1);
#else
    return (fifo->rd - fifo->wr - 1) & (fifo->size - 1);
#endif
}

// Занятое пространство в FIFO буфере
uint_fifo_t fifo_occup(fifo_t* fifo)
{
#ifdef __AVR__
    uint_fifo_t rd;
    uint_fifo_t wr;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
    {
        rd = fifo->rd;
        wr = fifo->wr;
    }
    return (wr - rd) & (fifo->size - 1);
#else
    return (fifo->wr - fifo->rd) & (fifo->size - 1);
#endif
}

// Записать данные в FIFO буфер
uint_fifo_t fifo_write(fifo_t* fifo, const void* ptr, uint_fifo_t len, uint_fifo_t offset)
{
    uint8_t* p = (uint8_t*) ptr;
    if ((len + offset) > fifo_free (fifo))
        return 0;

    uint_fifo_t wr;
#ifdef __AVR__
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
#endif
    {
        wr = fifo->wr;
    }
    wr = (wr + offset) & (fifo->size - 1);
    if (wr + len > fifo->size)
    {
        uint_fifo_t o = fifo->size - wr;
        memcpy (fifo->buff + wr, p, o);
        memcpy (fifo->buff, p + o, len - o);
    }
    else
        memcpy (fifo->buff + wr, p, len);
    return len;
}

// Положить данные в FIFO буфер
bool fifo_push (fifo_t* fifo, uint_fifo_t len)
{
    if (len > fifo_free (fifo))
        return false;
#ifdef __AVR__
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
    {
       fifo->wr = (fifo->wr + len) & (fifo->size - 1);
    }
#else
    fifo->wr = (fifo->wr + len) & (fifo->size - 1);
#endif
    return true;
}

// Положить текстовые данные в FIFO буфер
bool fifo_push_string (fifo_t* fifo, const char* str)
{
    uint_fifo_t len = strlen (str);
    if (len > fifo_free (fifo))
        return false;
    return fifo_write (fifo, str, len, 0) && fifo_push (fifo, len);
}

// Записать данные и зафиксировать новый размер в FIFO буфере
bool fifo_write_push (fifo_t* fifo, const void* ptr, uint_fifo_t len)
{
    return fifo_write (fifo, ptr, len, 0) && fifo_push (fifo, len);
}

// Прочитать данные из FIFO буфера
bool fifo_read (fifo_t* fifo, void* ptr, uint_fifo_t len, uint_fifo_t offset)
{
    uint8_t* p = (uint8_t*) ptr;
    if ((len + offset) > fifo_occup (fifo))
        return false;

    uint_fifo_t rd;
#ifdef __AVR__
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
#endif
    {
        rd = fifo->rd;
    }
    rd = (rd + offset) & (fifo->size - 1);
    if (rd + len > fifo->size)
    {
        uint_fifo_t o = fifo->size - rd;
        memcpy (p, fifo->buff + rd, o);
        memcpy (p + o, fifo->buff, len - o);
    }
    else
        memcpy (p, fifo->buff + rd, len);
    return true;
}

// Удалить данные из FIFO буфера
bool fifo_pop (fifo_t* fifo, uint_fifo_t len)
{
    if (len > fifo_occup (fifo))
        return false;
#ifdef __AVR__
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
    {
        fifo->rd = (fifo->rd + len) & (fifo->size - 1);
    }
#else
    fifo->rd = (fifo->rd + len) & (fifo->size - 1);
#endif
    return true;
}

// Прочитать и удалить данные из FIFO буфера
bool fifo_read_pop (fifo_t* fifo, void* ptr, uint_fifo_t len)
{
    return fifo_read (fifo, ptr, len, 0) && fifo_pop (fifo, len);
}

// Переместить данные из одного FIFO в другое
void fifo_move (fifo_t *d, fifo_t *s, uint_fifo_t len)
{
    if (fifo_free (d) < len)
        return;
    uint_fifo_t len_i;
    uint_fifo_t s_rd;
    uint_fifo_t d_wr;
#ifdef __AVR__
    // Обеспечиваем атомарность для AVR
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
#endif
    {
        s_rd = s->rd;
        d_wr = d->wr;
    }
    // Цикл копирований может выполняться до 4х раз
    while (len)
    {
        len_i = MIN (s->size - s_rd, d->size - d_wr);
        if (len_i > len)
            len_i = len;
        // Определяем размер копирования
        // Копируем
        memcpy (d->buff + d_wr, s->buff + s_rd, len_i);
        // Увеличиваем указатели
        s_rd = (s_rd + len_i) & (s->size - 1);
        d_wr = (d_wr + len_i) & (d->size - 1);
        len -= len_i;
    }
#ifdef __AVR__
    // Обеспечиваем атомарность для AVR
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
#endif
    {
        s->rd = s_rd;
        d->wr = d_wr;
    }
}
