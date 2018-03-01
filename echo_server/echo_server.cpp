#include <stdio.h>
#include <WinSock2.h>

void ErrorHandling(const char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

const unsigned int buf_size = 1024;
int main(int argc, char* argv[]) 
{
    WSAData wsaData;
    struct sockaddr_in serv_Addr, client_Addr;
    SOCKET serv_socket;
    SOCKET client_socket;
    char message[buf_size];
    int strLen;

    if (argc !=2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2,2),&wsaData) !=0)
    {
        ErrorHandling("WSAStartup() error!");
    }

    serv_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_socket == INVALID_SOCKET)
    {
        ErrorHandling("socket() error");
    }

    memset(&serv_Addr, 0, sizeof(serv_Addr));
    serv_Addr.sin_family = AF_INET;
    serv_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_Addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_socket,(SOCKADDR*)&serv_Addr,sizeof(serv_Addr)) == SOCKET_ERROR)
    {
        ErrorHandling("bind() error!");
    }
    if (listen(serv_socket,5) == SOCKET_ERROR)
    {
        ErrorHandling("listen() error");
    }
    
    int clientAddrSize = sizeof(client_Addr);

    for (size_t i = 0; i < 5; i++)
    {
        client_socket = accept(serv_socket, (SOCKADDR*)&client_Addr, &clientAddrSize);
        if (client_socket == -1)
        {
            ErrorHandling("accept() error");
        }
        else
        {
            printf("Connected client %d \n", i + 1);
        }
        while ((strLen = recv(client_socket,message,buf_size,0))!=0)
        {
            send(client_socket, message, strLen, 0);
        }
        closesocket(client_socket);
    }
    closesocket(serv_socket);
    WSACleanup();

    return 0;
}