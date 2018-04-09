#include <iostream>
#include <string>
#include <WinSock2.h>

const int buf_size = 30;
void error_handling(const std::string & message);

int main(int argc, char* argv[]) 
{
    WSAData wsaData;
    SOCKET sock;
    char message[buf_size];
    int str_len;
    int  adr_sz;
    SOCKADDR_IN server_addr, from_addr;

    if (argc != 3)
    {
        std::cout << "Usage : " << argv[0] << " <IP> <port> " << std::endl;
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
    {
        error_handling("WSAStartup() error");
    }
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        error_handling("socket() error");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    while (true)
    {
        std::cout << "Insert message(q to quit): ";
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
        {
            break;
        }
        sendto(sock, message, strlen(message), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        adr_sz = sizeof(from_addr);
        str_len = recvfrom(sock, message, buf_size, 0, (struct sockaddr*)&from_addr, &adr_sz);
        message[str_len] = 0;
        std::cout << message ;
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}



void error_handling(const std::string & message)
{
    std::cout << "Error: " << message << std::endl;
    exit(1);
}