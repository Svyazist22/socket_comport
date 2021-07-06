/**
 * @file utos.cpp
 * @author Vladislav Bakanov
 * @brief Слушает UART, проверяет полученную информацию и отправляет на соект результат
 * @version 0.1
 * @date 2021-07-06
 * @warning ШАБЛОН. Сейчас работает как client.
 */

#include "../../include/utos.h"

int main(int argc, char const *argv[])
{
    Client cl;
    cl.client_init();
    cl.client_write();

    return 0;
}
