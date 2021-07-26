#include <sys/socket.h>
#include <sys/types.h>  
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "../../../include/linux/transport_layer/client.h"

void Client::clientInit()
{
    Logger logger;

    // Создаем сокет-дескриптор
    sd = socket (AF_INET,SOCK_STREAM, 0); //ipv4 tsp

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (64300);

    // Преобразуем str в нужный формат
    inet_pton (AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Устанавливаем связь с сервером
    connectClient = connect (sd,(struct sockaddr *) &serv_addr, sizeof (serv_addr));
}

void Client::clientWrite (char* str)
{
    write(sd, str, strlen (str));
}

void Client::clientStop()
{
    close(sd);
}

Client::errorClient Client::getError ()
{
    if (sd == -1)
    {
        return errSd; 
    } 
    else if (connectClient == -1)
    {
        return errConn;
    }
    else
    {
        return errNo;
    }
}






