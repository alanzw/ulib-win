/*
 * =====================================================================================
 *
 *       Filename:  simple.cpp
 *
 *    Description:  simple socket example.
 *
 *        Version:  1.0
 *        Created:  2009-8-2 21:47:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <iostream>

#include "usocket.h"
#include "udllman.h"

// Definition of IP-Options
typedef struct tagIPINFO
{
  BYTE bTimeToLive;    //
  BYTE bTypeOfService; //
  BYTE bIpFlags;       //
  BYTE OptSize;        //
  BYTE FAR *Options;   //
} IPINFO, *PIPINFO;

// Definition of ICMP-Echo
typedef struct tagICMPECHO
{
    DWORD dwSource;     //
    DWORD dwStatus;     //
    DWORD dwRTTime;     //
    WORD  wDataSize;    //
    WORD  wReserved;
    void FAR *pData;    //
    IPINFO ipInfo;      //
} ICMPECHO, *PICMPECHO;

//
typedef HANDLE (WINAPI *PF_CMPCREATEFILE)(VOID);
typedef BOOL   (WINAPI *PF_ICMPCLOSEHANDLE)(HANDLE);
typedef DWORD  (WINAPI *PF_ICMPSENDECHO)(
    HANDLE,
    DWORD,
    LPVOID,
    WORD,
    PIPINFO,
    LPVOID,
    DWORD,
    DWORD);

int PingHost(const char *sAddress, int MaxRoundTime, int RoundTrips)
{
    //
    //HANDLE hIcmp = LoadLibrary("ICMP.DLL");
    //if(hIcmp == NULL)
    //    return -1;

    //
    //PF_CMPCREATEFILE pfIcmpCreateFile = (PF_CMPCREATEFILE) GetProcAddress(hIcmp, "IcmpCreateFile");
    //PF_ICMPCLOSEHANDLE pfIcmpCloseHandle = (PF_ICMPCLOSEHANDLE) GetProcAddress(hIcmp,"IcmpCloseHandle");
    //PF_ICMPSENDECHO pfIcmpSendEcho = (PF_ICMPSENDECHO) GetProcAddress(hIcmp,"IcmpSendEcho");

    //
    //if (pfIcmpCreateFile == NULL || pfIcmpCloseHandle == NULL || pfIcmpSendEcho == NULL)
    //{
    //    FreeLibrary(hIcmp);
    //    return -1;
    //}
    UDllMan udm("ICMP.DLL");

    PF_CMPCREATEFILE pfIcmpCreateFile = (PF_CMPCREATEFILE)udm.find("IcmpCreateFile");
    PF_ICMPCLOSEHANDLE pfIcmpCloseHandle = (PF_ICMPCLOSEHANDLE)udm.find("IcmpCloseHandle");
    PF_ICMPSENDECHO pfIcmpSendEcho = (PF_ICMPSENDECHO)udm.find("IcmpSendEcho");

    if (pfIcmpCreateFile == NULL || pfIcmpCloseHandle == NULL || pfIcmpSendEcho == NULL)
    {
        std::cout << "Faile To Find Proc!" << std::endl;
        return -1;
    }

    int ilRetVal = 0;
    //WSADATA wsaData;                              //
    //int ilRetVal = WSAStartup(0x0202, &wsaData );
    //if (ilRetVal)
    //{
    //    WSACleanup();
    //    FreeLibrary(hIcmp);
    //    return -1;
    //}

    //if (0x0202 != wsaData.wVersion)                // Check WinSock version
    //{
    //    WSACleanup();
    //    FreeLibrary(hIcmp);
    //    return -1;
    //}
    USocket us;
    us.init();


    //
    struct in_addr iaDest;                        //
    iaDest.s_addr = us.inet_addr(sAddress);
    LPHOSTENT pHost;                              //
    if (iaDest.s_addr == INADDR_NONE)
    {
 //       pHost = gethostbyname(sAddress);
        pHost = us.gethostbyname(sAddress);
    }
    else
    {
//        pHost = gethostbyaddr((const char*)&iaDest, sizeof(struct in_addr), AF_INET);
        pHost = us.gethostbyaddr((const char *)&iaDest, sizeof(struct in_addr), AF_INET);
    }
    if(pHost == NULL)
    {
//        WSACleanup();
//        FreeLibrary(hIcmp);
        return -1;
    }

    DWORD* pAddress = (DWORD*)(*pHost->h_addr_list);  // IP-Adresse kopieren
    HANDLE hIcmpFile = pfIcmpCreateFile();        //
    ICMPECHO icmpEcho;                            //
    IPINFO ipInfo;                                // IP-Optionenstruktur
    int ilTimeSum = 0;                            // Summe der Round Trip Time-Daten
    int ilCount   = 0;                            // Anzahl der Round Trip Time-Daten

    for (int ilPingNo = 0; ilPingNo < RoundTrips; ilPingNo++)
    {
        //
        ::ZeroMemory(&ipInfo, sizeof(ipInfo));
        ipInfo.bTimeToLive = MaxRoundTime;

        //
        pfIcmpSendEcho(hIcmpFile,        //
            *pAddress,                   //
            NULL,                        //
            0,                           //
            &ipInfo,                     //
            &icmpEcho,                   //
            sizeof(struct tagICMPECHO),  //
            MaxRoundTime);               //

        iaDest.s_addr = icmpEcho.dwSource;

        if(icmpEcho.dwStatus)
            break;

        ilTimeSum += icmpEcho.dwRTTime;
        ilCount++;
    }

    pfIcmpCloseHandle(hIcmpFile);                 //
    //FreeLibrary(hIcmp);                           //
    //WSACleanup();                                 //
    us.cleanup();

    if(ilTimeSum >= MaxRoundTime*ilCount)
    {
        std::cout << "Time Exceed.." << std::endl;
        return -1;
    }

    //
    return ilRetVal = ilCount ? ilTimeSum/ilCount : -1;
}

int main(int argc, char *argv[])
{
    std::cout << "Response : " <<  PingHost("127.0.0.1", 10000, 10) << "s" << std::endl;
    std::cout << "Response : " <<  PingHost("www.baidu.com", 10000, 10) << "s" << std::endl;
    return 0;
}

