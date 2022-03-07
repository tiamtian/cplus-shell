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
    char strmsg[256];
    char result[4096];
    recv(client, strmsg, sizeof(strmsg), 0);
    printf("%s\n", strmsg);
    strcat(strmsg, "... from server");
    send(client, strmsg, sizeof(strmsg), 0);
    while(1)
    {
        std::cout << "shell > ";
        std::cin >> strmsg;
        if(strcmp(strmsg, "exit") == 0)
        {
            return 0;
        }
        send(client, strmsg, sizeof(strmsg), 0);
        recv(client, result, sizeof(result), 0);
        std::cout << result << std::endl;
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