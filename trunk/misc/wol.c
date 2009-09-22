/*
**    Զ�̿���(����������)
**
**  ������� '
**
**    ���÷�Χ:

        ������

**  ʹ�÷���:

        �ڵ���ʹ�ø߷���
        C:>power --find 192.168.0.%d
        ��ȡ�����������������Ե�MAC��ַ�����浽MacData�ļ���
        ֻ��Ҫ����һ�μ���Ŷ '

        �ȵ���Ҳ�඼�ػ��� �ٺ� Ȼ�� ����һ��
        C:>power --startall
        ���Ծͻ���������������� ������ר�Ը��ػ��󲻹ص�Դ����~~~
        ��֪���Ļ���Ϊ�Լ����Գ�ë�����ء���Ȼ���Ӳ�� ��װ??..

**  ----�벻Ҫ�ڻ���ʹ�ñ����򡣡�����������Ա��æ��������...
**  ----�벻Ҫ������ʹ�ñ����򡣡������ܻ�ʧ�ߵ�...

**    ԭ��
        ����һ����������ݰ������� �������������֧�ֵĻ����ټ���BOIS
        ����Wake Up On Lan �ӵ��˰��ͻ��Զ�������
        (û�뵽��Ȼ����ô��Ĭ�����������õĵ��ԡ�����)

        ���ݰ���ʽ 6���ֽڵ�0xFF Ȼ������ظ�16�������MAC��ַ~~
**
*/

#include <windows.h>
#include <stdio.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

SOCKET wsa_init()
{
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0){
        printf("WSAStartup failed: %d ", GetLastError());
        return 0;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET){
        printf("Socket create error: %d ", GetLastError());
        return 0;
    }

    BOOL bOptVal = TRUE;
    int iOptLen = sizeof(BOOL);
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, iOptLen) == SOCKET_ERROR){
        printf("setsockopt error: %d ", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    return sock;
}

int wsa_clear(SOCKET sock)
{
    closesocket(sock);
    WSACleanup();
    return 0;
}

int send_magic(unsigned char *ether_addr, SOCKET sock)
{
    unsigned char magicpacket[200];

    sockaddr_in to;
    to.sin_family = AF_INET;
    to.sin_port = htons(0);
    to.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    memset(magicpacket, 0xff, 6);

    int packetsize=6;

    for(int i=0; i<16; i++)
    {
        memcpy(magicpacket + packetsize, ether_addr, 6);
        packetsize += 6;
    }

    if (sendto(sock, (const char *)magicpacket, packetsize, 0,
            (const struct sockaddr *)&to, sizeof(to)) == SOCKET_ERROR)
        printf("Magic packet send error: %d ", WSAGetLastError());
    else
        printf("Magic packet send! MAC: %02X-%02X-%02X-%02X-%02X-%02X ",
            ether_addr[0],ether_addr[1],ether_addr[2],ether_addr[3],ether_addr[4],ether_addr[5]);

    return 0;
}

int save_mac(unsigned char *addr){
    FILE *fp = fopen("MacData", "ab");
    if(fp){
        unsigned char ether_addr[6] = { 0 };
        memcpy(ether_addr, addr, 6);
        fwrite(ether_addr, 6, 1, fp);
        fclose(fp);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    bool bfind  = false; /**//* �Ƿ���������MAC��ַ */
    bool bstart = false; /**//* �Ƿ�Զ�̿��� */

    if(argc == 3 && strcmp(argv[1], "--find") == 0) {
        bfind = true;
    }else if (argc == 2 && strcmp(argv[1], "--startall") == 0) {
        bstart = true;
    }else{
        printf("USAGE: "
            "to find the mac use --find 192.168.0.%%d "
            "to start the computers you find use  --startall ");
        return 0;
    }

    char ip_maker[16];
    unsigned char ether_addr[6];
    unsigned long mac[2];
    unsigned long len;

    /**//*
        ����ģʽ����������
    */
    if (bfind){
        for (int l=0; l<=255; l++) {
            sprintf(ip_maker, argv[2], l);
            memset(mac, 0xFF, sizeof(mac));
            len = 6;
            unsigned long ip = inet_addr(ip_maker);
            /**//* SendARP ��ip��ȡmac��ַ */
            HRESULT hr = SendARP(ip, NULL, mac, &len);
            if (NO_ERROR == hr){
                for(int i=0; i<6; i++)
                    ether_addr[i] = ((unsigned char *)mac)[i];
                printf("ip : %s mac : %02X-%02X-%02X-%02X-%02X-%02X ", ip_maker,
                    ether_addr[0],ether_addr[1],ether_addr[2],ether_addr[3],ether_addr[4],ether_addr[5]);
                /**//* ����õ�mac��ַ���浽�ļ��б���' */
                save_mac(ether_addr);
            } else {
                continue;
            }
        }
    }
    /**//*
        ����ģʽ����������
    */
    if (bstart) {
        FILE *fp = fopen("MacData", "rb");
        if(fp){
            SOCKET sock = wsa_init();
            while(!feof(fp)){
                fread(ether_addr, 6, 1, fp);
                send_magic(ether_addr, sock);
            }
            fclose(fp);
            wsa_clear(sock);
        }else{
            printf("can not find file macdata, type --find [ip] ");
            return 0;
        }
    }

    //system("PAUSE");

    return 0;
}
