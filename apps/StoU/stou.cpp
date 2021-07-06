/**
 * @file stou.cpp
 * @author Vladislav Bakanov
 * @brief Слушает сокет, отправляет полученные данные на UART
 * @version 0.1
 * @date 2021-07-06
 * @warning ШАБЛОН. Сейчас работает как server
 */

#include "../../include/stou.h"

int main(int argc, char const *argv[])
{
    Server serv;
    
    serv.serv_init();
    serv.serv_read();
    
    
    return 0;
}
