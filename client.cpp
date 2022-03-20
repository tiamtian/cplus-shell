#ifndef __WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif
#include <string>
#include <stdio.h>
#include <iostream>
#include "FunCli.h"
#define PORT 4567
int main()
{
    int fd;
#ifndef __WIN32
    socklen_t sock_len;
#else
    int sock_len;
    WSADATA wsaData; // 初始化网络环境
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //使用2.2版本socket
    {
        printf("初始化失败！");
        return 0;
    }
#endif
    sockaddr_in clientAddr;
    char ip[16];
    sock_len = sizeof(clientAddr);
    fd = socket(AF_INET, SOCK_STREAM, 0); //创建TCP连接
    if(fd == -1)
    {
        printf("创建失败！");
        return 0;
    }
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT);
    printf("please enter IP address: ");
    scanf("%s", ip);
#ifndef __WIN32
    clientAddr.sin_addr.s_addr = inet_addr(ip);
#else
    clientAddr.sin_addr.S_un.S_addr = inet_addr(ip);
#endif
    if(connect(fd, (sockaddr *)&clientAddr, sock_len) == -1)
    {
        printf("连接失败！");
#ifndef __WIN32
        close(fd);
#else
        closesocket(fd);
#endif
    }
    const char *msg = "Im from client";
    send(fd, msg, strlen(msg) + 1, 0);
    char buffer[1024];
    memset(buffer, 0, 1024);

    recv(fd, buffer, 1024, 0);
    AES aes(buffer);
    int len;
    int error;
    while(1)
	{   
        error = recv(fd, buffer, 1024, 0);
        if(error == 0)break;
        aes.InvCipher((void *)buffer, 8);
        aes.Exec(buffer);

        len = strlen(aes.temp);
        aes.Cipher((void *)aes.temp);
        send(fd, aes.temp, 1024, 0);
	}
    memset(buffer, 0, 1024);

#ifndef __WIN32
    close(fd);
#else
    closesocket(fd);
    WSACleanup(); //清理网络环境
#endif
    return 0;

}