// 与hello_server_win 配合

#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
void ErrorHandling(const char* message);

int main(int argc,char* argv[]) 
{
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    char message[30];
    int strLen = 0;
    int idx = 0;
    int readLen = 0;

    if (argc !=3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //WSAStartup 初始化
    if (WSAStartup(MAKEWORD(2,2),&wsaData) !=0)
    {
        ErrorHandling("WSAStartup() error!");
    }

    //创建socket
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
    {
        ErrorHandling("socket() error!");
    }

    //设置 服务器地址
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    //connect
    if (connect(hSocket,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
    {
        ErrorHandling("connect() error!");
    }

    while (readLen = recv(hSocket, &message[idx++], 1, 0))
    {
        if (readLen == -1)
        {
            ErrorHandling("recv() error!");
        }
        strLen += readLen;
    }

    printf("Message from server: %s \n", message);
    printf("Function read call count:%d \n", strLen);
    closesocket(hSocket);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}