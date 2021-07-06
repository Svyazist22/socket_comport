/**
 * @file crypt.h
 * @author Vlad Bakanov
 * @brief Простое шифрование xor и получения хэша
 * @version 0.1
 * @date 2021-07-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Создание хэша
 * @param msg Сообщение из которого получается хэш
 * @param len Длина сообщения
 * @param hex Куда записывается хэш (размер хэша 8 байт)
 */
void create_hash(char *msg, size_t len, char *hex);

/**
 * @brief Шифрование XOR
 * @param data Текст для шифрования
 * @param key Ключ шифрования
 * @param dataLen Длина сообщения
 * @param keyLen Длина ключа
 * @return char* Возвращает указатель на зашифрованне сообщение
 */
char* XORCipher(char* data, char* key, int dataLen, int keyLen);

