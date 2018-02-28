//Winsock �ĳ�ʼ��
/*
 * WSAStartup���� �����ó������õ���Winsock�汾������ʼ����Ӧ�汾�Ŀ�
 *
 * #include <winsock2.h>
 * int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
 *  �ɹ�����0, ʧ�ܷ��ط���Ĵ������ֵ��
 *
 * wVersionRequested �õ���Winsocke �汾��Ϣ
 * lpWSAData WSADATA�ṹ������ĵ�ַ
 *
 * ʹ��MAKEWORD�꺯�� ���� WORD�Ͱ汾��Ϣ
 * MAKEWORD(2,2); //���汾Ϊ2�����汾Ϊ2������ 0x0202
*/

//Winsock ������  ע���ÿ�
/*
 * #include <winsock2.h>
 * int WSACleanup(void);
 *  �ɹ�����0, ʧ�ܷ��� SOCKET_ERROR
 *
*/

//socket   ����socket
/*
 * #include <winsock2.h>
 * SOCKET socket(int af, int type, int protocol);
 * �ɹ������׽��־����ʧ�ܷ��� INVALID_SOCKET
*/

// bind    ���÷���IP ��ַ �� �˿ں�
/*
 * #include <winsock2.h>
 * int bind(SOCKET s, const struct sockaddr * name, int namelen);
 * �ɹ�ʱ����0��ʧ��ʱ���� SOCKET_ERROR��
*/

//listen  ����ʹ�׽��ֿɽ��տͻ�������
/*
 * #include <winsock2.h>
 * int listen(SOCKET s, int backlog);
 * �ɹ�ʱ����0��ʧ��ʱ����SOCKET_ERROR
*/

//accept ���ܿͻ��˵���������
/*
 * #include <winsock2.h>
 * SOCKET accept(SOCKET s, struct sockaddr * addr, int * addrlen);
 * �ɹ�ʱ�����׽��־����ʧ�ܷ���INVALID_SOCKET��
*/

//connect ���ôӿͻ��˷�����������
/*
 * #include <winsock2.h>
 * int connect(SOCKET s, const struct sockaddr * name, int namelen);
 * �ɹ�ʱ����0��ʧ��ʱ����SOCKET_ERROR��
*/

//closesocket �ر��׽��ֺ���
/*
 * #include <winsock2.h>
 * int closesocket(SOCKET s);
 * �ɹ�ʱ����0��ʧ��ʱ����SOCKET_ERROR��
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

    //Winsock ��ʼ��
    if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        ErrorHandling("WSAStartup() error!");
    }

    //����socket
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
    {
        ErrorHandling("socket() error!");
    }

    //��������ַ
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
