/*
**    远程开机(局域网唤醒)
**
**  恶作剧版 '
**
**    适用范围:

        局域网

**  使用方法:

        在电脑使用高峰期
        C:>power --find 192.168.0.%d
        获取本局域网内其它电脑的MAC地址，保存到MacData文件中
        只需要运行一次即可哦 '

        等到大家差不多都关机了 嘿嘿 然后 运行一下
        C:>power --startall
        电脑就会神奇地启动起来了 哈哈，专对付关机后不关电源的人~~~
        不知道的还以为自己电脑出毛病了呢。。然后格硬盘 重装??..

**  ----请不要在机房使用本程序。。。机房管理员会忙不过来的...
**  ----请不要在网吧使用本程序。。。网管会失眠的...

**    原理：
        发送一个特殊的数据包给网卡 如果网卡和主板支持的话，再加上BOIS
        启用Wake Up On Lan 接到此包就会自动启动了
        (没想到居然有这么多默认设置是启用的电脑。。。)

        数据包格式 6个字节的0xFF 然后紧跟重复16遍的网卡MAC地址~~
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
    bool bfind  = false; /**//* 是否搜索机器MAC地址 */
    bool bstart = false; /**//* 是否远程开机 */

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
        搜索模式启动本程序
    */
    if (bfind){
        for (int l=0; l<=255; l++) {
            sprintf(ip_maker, argv[2], l);
            memset(mac, 0xFF, sizeof(mac));
            len = 6;
            unsigned long ip = inet_addr(ip_maker);
            /**//* SendARP 由ip获取mac地址 */
            HRESULT hr = SendARP(ip, NULL, mac, &len);
            if (NO_ERROR == hr){
                for(int i=0; i<6; i++)
                    ether_addr[i] = ((unsigned char *)mac)[i];
                printf("ip : %s mac : %02X-%02X-%02X-%02X-%02X-%02X ", ip_maker,
                    ether_addr[0],ether_addr[1],ether_addr[2],ether_addr[3],ether_addr[4],ether_addr[5]);
                /**//* 将获得的mac地址保存到文件中备用' */
                save_mac(ether_addr);
            } else {
                continue;
            }
        }
    }
    /**//*
        启动模式启动本程序
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
