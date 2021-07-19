#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#define FIFO_BITS 16

#if FIFO_BITS == 8
typedef uint8_t uint_fifo_t;
#elif FIFO_BITS == 16
typedef uint16_t uint_fifo_t;
#elif FIFO_BITS == 32
typedef uint32_t uint_fifo_t;
#endif


#pragma pack(push,1)
// Структура, описывающая FIFO
typedef struct
{
    uint_fifo_t rd;	// указатель чтения
    uint_fifo_t wr;	// указатель записи
    uint_fifo_t size;	// размер буфера
    uint8_t *buff;	// буфер FIFO
} fifo_t;
#pragma pack(pop)

 /**
 * @brief Инициализация FIFO
 * @param fifo Объект
 * @param buff_ptr Указатель на буфер, где будут находиться данные
 * @param buff_size Размер буфера, где будут находиться данные
 * @note buff_size должен быть кратен степени двойки
 */
void fifo_init (fifo_t* fifo, uint8_t *buff_ptr, uint_fifo_t buff_size);

/**
 * @brief Очистка буфера FIFO
 * @param fifo Объект
 */
void fifo_clear (fifo_t* fifo);

/**
 * @brief Вычисление свободного пространства буфера FIFO
 * @param fifo Объект
 * @return Число свободных байт
 */
uint_fifo_t fifo_free (fifo_t* fifo);

/**
 * @brief Вычисление занятого пространства буфера FIFO
 * @param fifo Объект
 * @return Число занятых байт
 */
uint_fifo_t fifo_occup (fifo_t* fifo);

/**
 * @brief Записать данные в FIFO
 * @param fifo Объект
 * @param ptr Указатель на данные
 * @param len Размер данных для записи
 * @param offset Смещение в буфере относительно текущего указателя записи (головы очереди)
 * @return Число фактически записанных байт
 */
uint_fifo_t fifo_write (fifo_t* fifo, const void* ptr, uint_fifo_t len, uint_fifo_t offset);

/**
 * @brief Положить данные в FIFO буфер
 * @param fifo Объект
 * @param len Размер данных, которые необходимо отметить "положенными"
 * @return true если это возможно было сделать
 */
bool fifo_push (fifo_t* fifo, uint_fifo_t len);

/**
 * @brief Положить текстовые данные в FIFO буфер
 * @param fifo Объект
 * @param str Строка для записи
 * @return true если это возможно было сделать
 */
bool fifo_push_string (fifo_t* fifo, const char* str);

/**
 * @brief Записать данные и зафиксировать новый размер в FIFO буфере
 * @param fifo Объект
 * @param ptr Указатель на данные
 * @param len Размер данных для записи
 * @return true если это возможно было сделать
 */
bool fifo_write_push (fifo_t* fifo, const void* ptr, uint_fifo_t len);

/**
 * @brief Прочитать данные из FIFO буфера
 * @param fifo Объект
 * @param ptr Указатель, куда будут прочитаны данные из fifo
 * @param len Размер данных для чтения
 * @param offset Смещение в буфере относительно текущего указателя чтения (хвоста очереди)
 * @return true если это возможно было сделать
 */
bool fifo_read (fifo_t* fifo, void* ptr, uint_fifo_t len, uint_fifo_t offset);

/**
 * @brief Удалить данные из FIFO буфера
 * @param fifo Объект
 * @param len Размер данных для удаления
 * @return true если это возможно было сделать
 */
bool fifo_pop (fifo_t* fifo, uint_fifo_t len);

/**
 * @brief Прочитать и удалить данные из FIFO буфера
 * @param fifo Объект
 * @param ptr Указатель, куда будут прочитаны данные из fifo
 * @param len Размер данных для чтения и последующего удаления из fifo
 * @return true если это возможно было сделать
 */
bool fifo_read_pop (fifo_t* fifo, void* ptr, uint_fifo_t len);

/**
 * @brief Переместить данные из одного FIFO в другое
 * @param d Объект fifo назначения
 * @param s Объект fifo источника
 * @param len Число перемещаемых байт данных
 */
void fifo_move (fifo_t *d, fifo_t *s, uint_fifo_t len);

#ifdef __cplusplus
}
#endif
