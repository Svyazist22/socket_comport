/**
 * @file utos.cpp
 * @author Vladislav Bakanov
 * @brief Слушает UART, проверяет полученную информацию и отправляет на соект результат
 * @version 0.1
 * @date 2021-07-06
 * @warning ШАБЛОН. Сейчас работает как client.
 */

#include "../../include/utos.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    char *str = new char;
    Client cl;
    cl.client_init();
    

    printf("Write command:");
    std::cin.getline(str,32);
    cl.client_write(str);


    return 0;
}
