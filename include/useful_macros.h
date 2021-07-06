/**
  @brief Useful macros
  @details Набор полезных макросов
  @author Nikita Shishkov
  @date 2021-04-30
  */

#pragma once

#include <stdint.h>

/// Выравнивание доступа к памяти
union __aliasing_through
{
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
}
#ifdef __GNUC__
  __attribute__((may_alias))
#endif
;

typedef union __aliasing_through __aliasing_through_t;

/// Получить little-endian uint32 из массива data со смещением ofs байт.
#define GET_LE32(data,ofs)	((__aliasing_through_t *)((char *)(data) + (ofs)))->u32
/// Записать little-endian uint32 в массива data по смещению ofs байт.
#define PUT_LE32(data,ofs,x)	(((__aliasing_through_t *)((char *)(data) + (ofs)))->u32 = (x))
/// Получить little-endian uint16 из массива data со смещением ofs байт.
#define GET_LE16(data,ofs)	((__aliasing_through_t *)((char *)(data) + (ofs)))->u16
/// Записать little-endian uint32 в массива data по смещению ofs байт.
#define PUT_LE16(data,ofs,x)	(((__aliasing_through_t *)((char *)(data) + (ofs)))->u16 = (x))

/// Длина статического массива в элементах
#define ARRAY_LEN(arr)		((sizeof (arr)) / (sizeof ((arr) [0])))
/// Размер элемента сатического массива в байтах
#define ITEM_SIZE(arr)		(sizeof ((arr) [0]))

/// Convert x -> "x"
#define _STRINGIFY(x)           #x
/// Convert x -> "x"
#define STRINGIFY(x)            _STRINGIFY(x)

/// Host-to-network
#define HTONS(x) ((((x) & 0xff) << 8) | (((x) & 0xff00) >> 8))
/// Network-to-host
#define NTOHS(x) HTONS(x)
/// Host-to-network
#define HTONL(x) ((((x) & 0xff) << 24) | \
                     (((x) & 0xff00) << 8) | \
                     (((x) & 0xff0000UL) >> 8) | \
                     (((x) & 0xff000000UL) >> 24))
/// Network-to-host
#define NTOHL(x) HTONL(x)
