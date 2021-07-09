#include "../include/server.h"

#include <sys/socket.h>
#include <sys/types.h>  
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>



int Server::sd;
int Server::bind_serv;
int Server::listen_serv;
int Server::accept_serv;




void Server::serv_init()
{
    Logger logger;
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
    Logger logger;
    char *buf = new char[1024];
    ssize_t nread = 0;

  
        nread = recv(accept_serv,buf,1024,0);
       
    
    //close(sd);
    //close(accept_serv);
    return buf;
}


