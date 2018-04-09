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
int calculate(int opnum, int opnds[],char oprator);
int main(int argc, char* argv[]) 
{
    if (argc !=2)
    {
        std::cout << "usage: <port> !" << std::endl;
    }
    WSAData wsaData;
    SOCKET server_sock;
    SOCKET client_sock;
    SOCKADDR_IN server_addr;
    SOCKADDR_IN client_addr;

    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
    {
        ErrorHandle("WSAStartup() error");
    }

    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == INVALID_SOCKET)
    {
        ErrorHandle("socket() error ");
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(ADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock,(SOCKADDR*)&server_addr,sizeof(server_addr)) == SOCKET_ERROR)
    {
        ErrorHandle("bind() error ");
    }
    if (listen(server_sock,5) == SOCKET_ERROR)
    {
        ErrorHandle("listen() error");
    }
    int clientAdrSize = sizeof(client_addr);
    int opndCnt = 0;
    int recvCnt = 0;
    int recvLen = 0;
    char opinfo[buf_Size];
    int result = 0;
    for (size_t i = 0; i < 5; i++)
    {
        opndCnt = 0;
        client_sock = accept(server_sock, (SOCKADDR*)&client_sock, &clientAdrSize);
        std::cout << "accept " << i + 1 << " ......" << std::endl;

        recv(client_sock, (char*)&opndCnt, 1, 0);
        std::cout << "recv--> opndCnt: " << opndCnt << " ......" << std::endl;
        recvLen = 0;
        while ((opndCnt*opsz+1)>recvLen)
        {
            recvCnt = recv(client_sock, &opinfo[recvLen], buf_Size - 1, 0);
            if (recvCnt <0)
            {
                break;
            }
            recvLen += recvCnt;
            std::cout << "recvLen  -->" << recvLen << std::endl;
        }
        result = calculate(opndCnt, (int*)opinfo, opinfo[recvLen - 1]);
        send(client_sock, (char*)&result, sizeof(result),0);
        closesocket(client_sock);

    }

    closesocket(server_sock);
    WSACleanup();
    return 0;
}

int calculate(int opnum, int opnds[], char oprator)
{
    int result = opnds[0];
    switch (oprator)
    {
    case '+':
        for (int i = 0; i < opnum; i++)
        {
            std::cout << i << " : " << opnds[i] << std::endl;
            result += opnds[i];
        }
        break;
    case '-':
        for (int i = 0; i < opnum; i++)
        {
            result -= opnds[i];
        }
        break;
    case '*':
        for (int i = 0; i < opnum; i++)
        {
            result *= opnds[i];
        }
        break;
    default:
        break;
    }
    return result;
}
