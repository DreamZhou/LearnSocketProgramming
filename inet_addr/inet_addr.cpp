#include <stdio.h>
#include <WinSock2.h>
//#include <arpa/inet.h>

void ErrorHandling(const char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) 
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        ErrorHandling("WSAStartup() error!");
    }


    const char* addr1 = "1.2.3.4";
    const char* addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE)
    {
        printf("Error occured! \n");
    }
    else
    {
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE)
    {
        printf("Error occureded \n");
    }
    else
    {
        printf("Network ordered integer addr: %#lx \n\n", conv_addr);
    }

    struct sockaddr_in addr;
    char* strPtr;
    char  strArr[20];

    addr.sin_addr.s_addr = htonl(0x1020304);
    strPtr = inet_ntoa(addr.sin_addr);
    strcpy(strArr, strPtr);
    printf("Dotted-Decimal notation3 %s \n", strArr);

    WSACleanup();
    return 0;
}