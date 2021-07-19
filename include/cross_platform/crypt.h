/**
 * @file crypt.h
 * @author Vladislav Bakanov
 * @brief Простое шифрование xor и получения хэша
 * @version 0.1
 * @date 2021-07-02
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Создание хэша cообщения (размер хэша 16 символов)
 * @param [in] msg      Указатель на сообщение
 * @param [in] len      Длина сообщения
 * @param [in,out] hex  Указатель на массив для записи хэша 
 */
void create_hash(char *msg, size_t len, char *hex);


/**
 * @brief Сравнивнение двух хэшей
 * @param [in] h1 Указатель на первый хэш
 * @param [in] h2 Указатель на второй хэш
 * @return true если они одинаковые, иначе false
 */
bool compare_hash(char* h1,char* h2);

