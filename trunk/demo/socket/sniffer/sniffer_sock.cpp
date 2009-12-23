/*
 * =====================================================================================
 *
 *       Filename:  simple.cpp
 *
 *    Description:  a simple socket example.
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
#include "resource.h"
 
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include "usocket.h"

// *** Prototypes
void translate_ip(DWORD _ip, char *_cip);
void decode_tcp(char *_packet);
void decode_icmp(char *_packet);
void get_this_machine_ip(char *_retIP);

// *** Defines and Typedefs

#define LS_HI_PART(x)  ((x>>4) & 0x0F)
#define LS_LO_PART(x)  ((x) & 0x0F)

#define LS_MAX_PACKET_SIZE 65535

#ifndef SIO_RCVALL
#  define SIO_RCVALL    _WSAIOW(IOC_VENDOR,1)
#endif

typedef struct _IP_HEADER_
{
   BYTE  ver_ihl;        // Version (4 bits) and Internet Header Length (4 bits)
   BYTE  type;           // Type of Service (8 bits)
   WORD  length;         // Total size of packet (header + data)(16 bits)
   WORD  packet_id;      // (16 bits)
   WORD  flags_foff;     // Flags (3 bits) and Fragment Offset (13 bits)
   BYTE  time_to_live;   // (8 bits)
   BYTE  protocol;       // (8 bits)
   WORD  hdr_chksum;     // Header check sum (16 bits)
   DWORD source_ip;      // Source Address (32 bits)
   DWORD destination_ip; // Destination Address (32 bits)
} IPHEADER;

typedef struct _TCP_HEADER_
{
   WORD  source_port;       // (16 bits)
   WORD  destination_port;  // (16 bits)
   DWORD seq_number;        // Sequence Number (32 bits)
   DWORD ack_number;        // Acknowledgment Number (32 bits)
   WORD  info_ctrl;         // Data Offset (4 bits), Reserved (6 bits), Control bits (6 bits)
   WORD  window;            // (16 bits)
   WORD  checksum;          // (16 bits)
   WORD  urgent_pointer;    // (16 bits)
} TCPHEADER;

typedef struct _ICMP_HEADER_
{
   BYTE type;               // (8 bits)  
   BYTE code;               // (8 bits)  
   WORD checksum;           // (16 bits)  
} ICMPHEADER;

void get_this_machine_ip(char *_retIP)
{
   char host_name[128];
   struct hostent *hs;
   struct in_addr in;

   memset( host_name, 0x00, sizeof(host_name) );
   gethostname(host_name,128);
   hs = gethostbyname(host_name);

   memcpy( &in, hs->h_addr, hs->h_length );
   strcpy( _retIP, inet_ntoa(in) );
}

void translate_ip(DWORD _ip, char *_cip)
{
   struct in_addr in;

	in.S_un.S_addr = _ip;
	strcpy( _cip, inet_ntoa(in) );
}

void decode_tcp(char *_packet)
{
   TCPHEADER *tcp_header = (TCPHEADER *)_packet;
   BYTE flags = ( ntohs(tcp_header->info_ctrl) & 0x003F ); 

   printf("\n         source port      : %ld", htons(tcp_header->source_port));
   printf("\n         destination port : %ld", htons(tcp_header->destination_port));
   printf("\n         control bits     : ");

   if ( flags & 0x01 ) // FIN
      printf( "FIN " );

   if ( flags & 0x02 ) // SYN
      printf( "SYN " );

   if ( flags & 0x04 ) // RST
      printf( "RST " );

   if ( flags & 0x08 ) // PSH
      printf( "PSH " );

   if ( flags & 0x10 ) // ACK
      printf( "ACK " );

   if ( flags & 0x20 ) // URG
      printf( "URG " );

   printf("\n         sequence number  : %lu", ntohl(tcp_header->seq_number));
}

void decode_icmp(char *_packet)
{
   ICMPHEADER *icmp_header = (ICMPHEADER *)_packet;

   printf("\n         type: %d", icmp_header->type );
   
   switch ( icmp_header->type )
   {
      case 0:
         printf( " (echo reply)" );
         break;

      case 8:
         printf( " (echo request)" );
         break;

      default:
         break;
   }

   printf("\n         code: %d", icmp_header->code );
   printf("\n         code: %ld", icmp_header->checksum );
}

int main(int argc, char *argv[])
{
   IPHEADER *ip_header = NULL;
   int      optval = 1;
   DWORD    dwLen = 0;
   char     packet[LS_MAX_PACKET_SIZE];
   int      iRet = 0;
   int      ip_header_size = 0;
   char     ipSrc[20], ipDest[20], thisIP[20];
   BOOL     bShowTCP = TRUE, bShowICMP = TRUE;
 
   // Check arguments
   if ( _argc > 1 )
   {
      if ( !_stricmp(argv[1], "icmp") )
         bShowTCP = FALSE;
      else if ( !_stricmp(argv[1], "tcp") )
         bShowICMP = FALSE;
      else
      {
         printf( "\nUsage lsniff [ICMP|TCP]\n" );
         exit(0);
      }
    }
    
    USocket us;
    us.init();
    
    us.create(AF_INET, SOCK_RAW, IPPROTO_IP);
     
     memset( thisIP, 0x00, sizeof(thisIP) );
     get_this_machine_ip(thisIP);
    struct   sockaddr_in sock_sniff;
	sock_sniff.sin_family = AF_INET;
	sock_sniff.sin_port = htons(0);
   // If your machine has more than one IP you might put another one instead thisIP value
	sock_sniff.sin_addr.s_addr = inet_addr(thisIP);

    us.bind((struct sockaddr *)&sock_sniff);

      // Set socket to promiscuous mode
    // setsockopt wont work ... dont even try it
    if ( WSAIoctl( us,
                  SIO_RCVALL,
                  &optval,
                  sizeof(optval),
                  NULL,
                  0,
                  &dwLen,
                  NULL,
                  NULL ) == SOCKET_ERROR )

	{
        printf( "Error: WSAIoctl  = %ld\n", WSAGetLastError() );
		exit(-3);
	}

    while ( TRUE )
    {
        memset( packet, 0x00, sizeof(packet) );

        iRet = us.recv( packet, LS_MAX_PACKET_SIZE);
        if ( iRet < sizeof(IPHEADER) )
            continue;

        ip_header = (IPHEADER *)packet;

        // I only want IPv4 not IPv6
        if ( LS_HI_PART(ip_header->ver_ihl) != 4 ) 
            continue;

        ip_header_size = LS_LO_PART(ip_header->ver_ihl);
        ip_header_size *= sizeof(DWORD); // size in 32 bits words

        // Checks the protocol IP is encapsulating
        memset( ipSrc, 0x00, sizeof(ipSrc) );
        memset( ipDest, 0x00, sizeof(ipDest) );
        translate_ip(ip_header->source_ip, ipSrc);
        translate_ip(ip_header->destination_ip, ipDest);

        // Read http://www.ietf.org/rfc/rfc1700.txt?number=1700
        switch( ip_header->protocol )
        {
            case 1: // ICMP
                if ( bShowICMP )
                {
                    printf("\n -------------------- // -------------------- ");
                    printf("\n IP Header:");
                    printf("\n   Source      IP: %s", ipSrc);
                    printf("\n   Destination IP: %s", ipDest);
                    printf("\n      ICMP Header:");

                    decode_icmp(&packet[ip_header_size]);
                }
                break;

            case 6: // TCP
                if ( bShowTCP )
                {
                    printf("\n -------------------- // -------------------- ");
                    printf("\n IP Header:");
                    printf("\n   Source      IP: %s", ipSrc);
                    printf("\n   Destination IP: %s", ipDest);
                    printf("\n      TCP Header:");

                    decode_tcp(&packet[ip_header_size]);
                }
                break;

            case 17: // UDP
                break;

            default:
                break;
        }
   
    } // end-while
    
    us.cleanup();
    
    getchar();

    return 0;
}

