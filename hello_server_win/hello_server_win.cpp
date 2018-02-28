//Winsock 的初始化
/*
 * WSAStartup函数 ，设置程序中用到的Winsock版本，并初始化相应版本的库
 *
 * #include <winsock2.h>
 * int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
 *  成功返回0, 失败返回非零的错误代码值。
 *
 * wVersionRequested 用到的Winsocke 版本信息
 * lpWSAData WSADATA结构体变量的地址
 *
 * 使用MAKEWORD宏函数 构建 WORD型版本信息
 * MAKEWORD(2,2); //主版本为2，副版本为2，返回 0x0202
*/

//Winsock 的清理  注销该库
/*
 * #include <winsock2.h>
 * int WSACleanup(void);
 *  成功返回0, 失败返回 SOCKET_ERROR
 *
*/

//socket   创建socket
/*
 * #include <winsock2.h>
 * SOCKET socket(int af, int type, int protocol);
 * 成功返回套接字句柄，失败返回 INVALID_SOCKET
*/

// bind    调用分配IP 地址 和 端口号
/*
 * #include <winsock2.h>
 * int bind(SOCKET s, const struct sockaddr * name, int namelen);
 * 成功时返回0，失败时返回 SOCKET_ERROR。
*/

//listen  调用使套接字可接收客户端连接
/*
 * #include <winsock2.h>
 * int listen(SOCKET s, int backlog);
 * 成功时返回0，失败时返回SOCKET_ERROR
*/

//accept 接受客户端的连接请求
/*
 * #include <winsock2.h>
 * SOCKET accept(SOCKET s, struct sockaddr * addr, int * addrlen);
 * 成功时返回套接字句柄，失败返回INVALID_SOCKET。
*/

//connect 调用从客户端发送连接请求
/*
 * #include <winsock2.h>
 * int connect(SOCKET s, const struct sockaddr * name, int namelen);
 * 成功时返回0，失败时返回SOCKET_ERROR。
*/

//closesocket 关闭套接字函数
/*
 * #include <winsock2.h>
 * int closesocket(SOCKET s);
 * 成功时返回0，失败时返回SOCKET_ERROR。
*/

#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
void ErrorHandling(const char* message);

int main(int argc, char* argv[]) 
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;

    int szClntAddr;
    char message[] = "Hello World!";
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    //Winsock 初始化
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        ErrorHandling("WSAStartup() error!");
    }

    //创建socket
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
    {
        ErrorHandling("socket() error!");
    }

    //服务器地址
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    //bind
    if (bind(hServSock,(SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
    {
        ErrorHandling("bind() error!");
    }

    //listen 
    if (listen(hServSock,5) == SOCKET_ERROR)
    {
        ErrorHandling("listen() error!");
    }

    //accept
    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

    if (hClntSock == INVALID_SOCKET)
    {
        ErrorHandling("accept() error");
    }

    //send
    send(hClntSock, message, sizeof(message), 0);
 
    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup();

    return 0;
}

void ErrorHandling( const char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
