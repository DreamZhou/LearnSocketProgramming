#include <WinSock2.h>
#include <iostream>
#include <string>

const int buf_size = 30;

void error_handling(const std::string & message);

int main(int argc, char*argv[]) 
{
    if (argc !=2)
    {
        std::cout << "Usage: " << argv[0] << " <port> " << std::endl;
        exit(1);
    }

    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        error_handling("WSAStartup() error!");
    }
    int server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_sock == INVALID_SOCKET)
    {
        error_handling("socket() error!");
    }

    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock,(SOCKADDR*)&server_addr,sizeof(server_addr)) == SOCKET_ERROR)
    {
        error_handling("bind() error!");
    }

    SOCKADDR_IN client_addr;
    char message[buf_size];
    while (true)
    {
        int client_addr_size = sizeof(client_addr);
        int str_len = recvfrom(server_sock, message, buf_size, 0,
            (SOCKADDR*)&client_addr, &client_addr_size);
        sendto(server_sock, message, str_len, 0, (SOCKADDR*)&client_addr, client_addr_size);
    }
    closesocket(server_sock);
    WSACleanup();
    return 0;
}


void error_handling(const std::string & message)
{
    std::cout << "Error: " << message << std::endl;
    exit(1);
}
