#include <WinSock2.h>
#include <string>
#include <iostream>

const int buf_Size = 1024;
const int rlt_Size = 4;
const int opsz = 4;

void ErrorHandle(const std::string& errorStr)
{
    std::cout << "Error: " << errorStr << " ." << std::endl;
    exit(1);
}

int main(int argc,char* argv[]) 
{
    WSADATA wsaData;
    SOCKET hSocket;
    char opmsg[buf_Size];
    int result;
    int opndCnt;
    int i;
    SOCKADDR_IN servAdr;

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <IP> <port> " << std::endl;
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        ErrorHandle("WSAStartup() error!");
    }
    hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hSocket == INVALID_SOCKET)
    {
        ErrorHandle("socket() error!");
    }
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));

    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
    {
        ErrorHandle("connect() error!");
    }
    else
    {
        std::cout << "Connected ........." << std::endl;
    }
    std::cout << "Operand count: " ;
    scanf("%d", &opndCnt);
    opmsg[0] = (char)opndCnt;

    for ( i = 0; i < opndCnt; i++)
    {
        std::cout << "Operand "<<i+1<<": ",
            scanf("%d", (int*)&opmsg[i*opsz + 1]);
    }
    fgetc(stdin);
    std::cout << "Operator: " ;
    scanf("%c", &opmsg[opndCnt*opsz + 1]);
    std::cout << "before send...." << std::endl;

    send(hSocket, opmsg, opndCnt*opsz + 2, 0);

    std::cout << "after send...." << std::endl;
    recv(hSocket, (char*)&result, rlt_Size,0);

    std::cout << "Operation result: " << result << std::endl;

    closesocket(hSocket);
    WSACleanup();

    return 0;
}