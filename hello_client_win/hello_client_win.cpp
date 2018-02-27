// send 函数
/*
 * #include <winsock2.h>
 * int send(SOCKET s, const char * buf, int len, int flags);
 * 成功时返回传输字节数，失败时返回SOCKET_ERROR。
*/

// recv 函数
/*
 * #include <winsock2.h>
 * int recv(SOCKET s, const char* buf, int len, int flags);
 * 成功时返回接受的字节数（收到EOF时为0),失败返回SOCKET_ERROR。
*/










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
    int strLen;
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

    strLen = recv(hSocket, message, sizeof(message) - 1, 0);
    if (strLen == 1)
    {
        ErrorHandling("recv() error!");
    }
    printf("Message from server: %s \n", message);
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