#include "../include/server.h"

#include <sys/socket.h>
#include <sys/types.h>  
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

Logger logger;

int Server::sd;
int Server::bind_serv;
int Server::listen_serv;
int Server::accept_serv;

void Server::serv_init()
{
    // Создаем сокет сокет-дескриптор
    sd = socket(AF_INET,SOCK_STREAM,0); //ipv4 tsp
    if (sd ==-1)
    {
        logger.err("Error create socket");
        exit(0);
    }
    
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET; 
    // serv_addr.sin_addr.s_addr = inet_addr("");
    serv_addr.sin_port = htons(PORT); //порт 64300

    // Связываем сокет с конкретным адресом
    bind_serv = bind(sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)); 
    if (bind_serv ==-1)
    {
        logger.err("Error create bind");
        exit(0);
    }

    // Слушаем конкретный сокет
    listen_serv = listen(sd,5);
    if (listen_serv ==-1)
    {
        logger.err("Error listen");
        exit(0);
    }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    // Принятие связи на сокет. Возвращает новый сокет-дескриптор, через который происходит 
    //общение клиента с сервером
    accept_serv = accept(sd,(struct sockaddr*)&client_addr,&length);
    if (accept_serv ==-1)
    {
        logger.err("Error accept");
        exit(0);
    }
}

char* Server::serv_read()
{
    char *buf = new char;
    ssize_t nread;

    /*!
    Бесконечное чтение

    while (1)
    {
        nread = read(accept_serv,&buf,256);
        if (nread > 0)
        {
            logger.info("%s",buf);
            write(accept_serv,&buf,nread);
        }
    }
    */

    //Слшушаем что передает клиент
    nread = read(accept_serv,buf,256);
    if (nread < 0)
    {
        logger.err("Error read");
        exit(0);
    }
    /*
    if (nread > 0)
    {
        logger.info("%s",buf);
        write(accept_serv,buf,nread);
    }
    */
    
    sleep(1);
    close(sd);
    close(listen_serv);
    return buf;
}


