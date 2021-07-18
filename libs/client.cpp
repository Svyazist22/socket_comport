#include "../include/client.h"

#include <sys/socket.h>
#include <sys/types.h>  
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>


Client::error_client Client::client_init()
{
    Logger logger;

    // Создаем сокет сокет-дескриптор
    sd = socket(AF_INET,SOCK_STREAM,0); //ipv4 tsp
    if (sd ==-1)
    {
        logger.err("Error create socket: %s",strerror(errno));
        return err_sd;
    }
 
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(64300);

    // Преобразует str в нужный формат
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);

    // Установление связи с сервером
    connect_client = connect(sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if (connect_client ==-1)
    {
        logger.err("Error connect: %s",strerror(errno));
        return err_conn;
    }

    return err_no;
}

void Client::client_write(char* str)
{
    write(sd,str,strlen(str));
}

void Client::client_stop()
{
    close(sd);
}







