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
    SOCKET clientSocket;
    SOCKADDR_IN servAddr;
    char message[buf_size];

    if (argc !=3)
    {
        printf("Usage %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
    {
        ErrorHandling("WSAStartup() error!");
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket==INVALID_SOCKET)
    {
        ErrorHandling("socket() error!");
    }


    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if (connect(clientSocket,(SOCKADDR*)&servAddr,sizeof(servAddr))==SOCKET_ERROR)
    {
        ErrorHandling("connect() error!");
    }
    else
    {
        printf("Connect...\n");
    }

    int strLen = 0;

    while (true)
    {
        memset(message, 0, buf_size);
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, buf_size, stdin);

        if (!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
        {
            break;
        }
        printf("message --> %s", message);
        send(clientSocket, message, strlen(message), 0);
        strLen = recv(clientSocket, message, buf_size - 1, 0);
        message[strLen] = 0;
        printf("Message from server: %s", message);
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}