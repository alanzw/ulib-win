#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>

#include <tchar.h>

using namespace std;

#define USERADD 1
#define USERGET	2
#define USERDEL	3
#define GETNODE	4
#define BLOCKSIZE	1024

typedef int CMD;

typedef struct
{	
	char	user[20];
	char	ip[16];
	int		port;
}userInfo;

typedef struct
{
	char	ip[16];	
	int		port;
	int		userNum;
	_off_t	totalsize;
	_off_t	freesize;
}nodeInfo;

typedef struct
{
	char	ip[16];
	int		port;
}nodeaddr;


FILE *fp_user;
FILE *fp_node;

int nodeadd(char* ip, int port, _off_t	totalsize)
{
	nodeInfo *temp = (nodeInfo *) malloc (sizeof(nodeInfo));
	fseek(fp_node, 0, 0);
	while( fread(temp, sizeof(nodeInfo), 1, fp_node) == 1) //检索是否已经存在
	{
		if(strcmp(temp->ip, ip) == 0 && temp->port == port)
		{
			free(temp);
			return 0;
		}
	}

	strcpy(temp->ip, ip);
	temp->port = port;
	temp->userNum = 0;
	temp->totalsize = totalsize;
	temp->freesize = totalsize;
	fwrite(temp, sizeof(nodeInfo), 1, fp_node);
	fseek(fp_node, 0, 0);
	fflush(fp_node);
	free(temp);
	return 1;
}


int usergetnode(nodeInfo *node)
{
	nodeInfo *temp = (nodeInfo *) malloc (sizeof(nodeInfo));
	fseek(fp_node, 0, 0);
	int num = 0;
	int no = 0;
	if(fread(node, sizeof(nodeInfo), 1, fp_node) != 1)
		return 0;
	while( fread(temp, sizeof(nodeInfo), 1, fp_node) == 1)
	{	
		++num;
		if(temp->userNum < node->userNum)
		{
			*node = *temp;
			no = num;
		}

	}

	memset(temp, 0, sizeof(nodeInfo));
	*temp = *node;
	temp->userNum += 1;
	fseek(fp_node, no*sizeof(nodeInfo), 0);
	fwrite(temp, sizeof(nodeInfo), 1, fp_node);
	
	fseek(fp_node, 0, 0);
	fflush(fp_node);
	free(temp);
	
	return 1;
}


int	useradd(char* username)
{
	userInfo *temp = (userInfo *) malloc (sizeof(userInfo));
	nodeInfo *node = (nodeInfo *) malloc (sizeof(nodeInfo));
	fseek(fp_user, 0, 0);
	while( fread(temp, sizeof(userInfo), 1, fp_user) == 1)
		if(strcmp(temp->user, username) == 0)
		{
			free(temp);
			free(node);
			return 0;	//该用户名已经存在
		}

	if(usergetnode(node) == 0)
		return -1;	//获取节点失败

	memset(temp, 0, sizeof(userInfo));
	strcpy(temp->user, username);
	strcpy(temp->ip, node->ip);
	temp->port = node->port;
	fwrite(temp, sizeof(userInfo), 1, fp_user);
	fseek(fp_user, 0, 0);
	fflush(fp_user);
	free(temp);
	free(node);
	return 1;
}

int userget(char* username, userInfo *user)
{
	userInfo *temp = (userInfo *) malloc (sizeof(userInfo));
	fseek(fp_user, 0, 0);
	while( fread(temp, sizeof(userInfo), 1, fp_user) == 1)
		if(strcmp(temp->user, username) == 0)
		{
			*user = *temp;
			return 1;
		}
	return 0;
}

int datagetnode(nodeInfo *node)
{
	
	nodeInfo *temp = (nodeInfo *) malloc (sizeof(nodeInfo));
	fseek(fp_node, 0, 0);
	int num = 0;
	int no = 0;
	if(fread(node, sizeof(nodeInfo), 1, fp_node) != 1)
		return 0;
	while( fread(temp, sizeof(nodeInfo), 1, fp_node) == 1)
	{	
		++num;
		if(temp->freesize > node->freesize)
		{
			*node = *temp;
			no = num;
		}

	}

	memset(temp, 0, sizeof(nodeInfo));
	*temp = *node;
	temp->freesize -= BLOCKSIZE;
	fseek(fp_node, no*sizeof(nodeInfo), 0);
	fwrite(temp, sizeof(nodeInfo), 1, fp_node);
	
	fseek(fp_node, 0, 0);
	fflush(fp_node);
	free(temp);
	
	return 1;
}

void openfile()
{
	if( (fp_user = fopen("root.dat", "rb+")) == NULL)
		fp_user = fopen("root.dat", "wb+");
	if( (fp_node = fopen("node.dat", "rb+")) == NULL)
		fp_node = fopen("node.dat", "wb+");

}


unsigned _stdcall useraddthread(LPVOID p)
{
	SOCKET*	tsock = (SOCKET *)p;
	struct sockaddr_in taddr;
	int len = 20;
	char* username = (char *) malloc (len * sizeof(char));
	recv(*tsock, username, len, 0);
	int result = useradd(username);
	send(*tsock, (char *)&result, sizeof(int), 0);

	closesocket(*tsock);
	free(username);
	free(tsock);
	return 1;
}
unsigned _stdcall usergetthread(LPVOID p) //客户端获取用户信息文件的存放地址
{
	SOCKET*	tsock = (SOCKET *)p;
	struct sockaddr_in taddr;
	int len = 20;
	char* username = (char *) malloc (len);
	userInfo *user = (userInfo *) malloc (sizeof(userInfo));

	recv(*tsock, username, len, 0);
	cout << "接收到的用户名:" << username << endl;
	if(userget(username, user) == 0)
	{
		cout << "get false" << endl;
		strcpy(user->user, "0");
		strcpy(user->ip,"0.0.0.0");
		user->port = 0;
	}
	cout << "int_thread:" << user->user << "    " << user->ip << ":" << user->port << endl;
	send(*tsock, (char *)user, sizeof(userInfo), 0);


	closesocket(*tsock);
	free(username);
	free(user);

	return 1;
}

unsigned _stdcall userdelthread(LPVOID p)
{
	return 1;
}


unsigned _stdcall getnodeaddrthread(LPVOID p) //为数据块分配节点，发送给客户端
{
	SOCKET*	tsock = (SOCKET *)p;
	int blocknum = 0;
	nodeInfo *node = (nodeInfo *) malloc (sizeof(nodeInfo));
	nodeaddr *naddr = (nodeaddr *) malloc (sizeof(nodeaddr));

	recv(*tsock, (char *)&blocknum, sizeof(int), 0); //获取块个数
	for(int i = 0; i < blocknum; i++)
	{
		memset(node, 0, sizeof(nodeInfo));
		memset(naddr,0, sizeof(nodeaddr));
		datagetnode(node);
		naddr->port = node->port;
		strcpy(naddr->ip, node->ip);
		send(*tsock, (char *)naddr, sizeof(nodeaddr), 0); //发送节点地址
	}
	
	closesocket(*tsock);
	free(node);
	free(naddr);
	free(tsock);

	return 1;
}

void test();

int _tmain(int argc, _TCHAR* argv[])
{
	openfile();
	test();

	WSADATA wsa;
	int ret = 0;
	int PORT = 5100;
	SOCKET	s_socket;
	SOCKET	c_socket;
	struct sockaddr_in	s_sockaddr;
	struct sockaddr_in	c_sockaddr;
	int	c_sockaddr_len = sizeof(c_sockaddr);


	ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	if(ret != 0)
	{
		cout << "Init WinSock failed:"  << ret << endl;
		return 0;
	}

	if((s_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
	{
		cout << "Create Socket Failed:" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	s_sockaddr.sin_family = AF_INET;
	s_sockaddr.sin_port = htons(PORT);
	s_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(s_socket, (SOCKADDR *)&s_sockaddr, sizeof(s_sockaddr)) == SOCKET_ERROR)
	{
		cout << "bind failed:" <<  WSAGetLastError() << endl;
		closesocket(s_socket);
		WSACleanup();
		return 0;
	}

	if(listen(s_socket, 5) == SOCKET_ERROR)
	{
		cout << "Listen Failed:" <<  WSAGetLastError() << endl;
		closesocket(s_socket);
		WSACleanup();
		return 0;
	}


	CMD cmd;
	while(1)
	{
		c_socket = accept(s_socket, (SOCKADDR *)&c_sockaddr, &c_sockaddr_len);
		recv(c_socket, (char *)&cmd, sizeof(CMD), 0);
		SOCKET *tsock = (SOCKET *)malloc(sizeof(SOCKET));
		*tsock = c_socket;

		switch(cmd)
		{
		case USERADD:
			_beginthreadex(NULL, 0, &useraddthread, tsock, 0, NULL);
			break;
		case USERGET:
			_beginthreadex(NULL, 0, &usergetthread, tsock, 0, NULL);
		case USERDEL:
			_beginthreadex(NULL, 0, &userdelthread, tsock, 0, NULL);
		case GETNODE:
			_beginthreadex(NULL, 0, &getnodeaddrthread, tsock, 0, NULL);
		default:
			break;
		}
	}
	cin.get();
	cin.get();
	return 0;
}






void test()
{
	cout << "输入地址信息" << endl;
	char ip[16];
	int port = 5101;
	while(1)
	{
		cout << "输入IP" << endl;
		scanf("%s",ip);
		if(strcmp(ip,"0") == 0)
			break;
		if(nodeadd(ip, port, 1024*10) == 0)
			cout << "该记录已经存在" << endl;
	}

	cout << "主服务器配置完成" << endl;
}

/*
void test()
{
	char ip[16];
	int port;
	strcpy(ip, "127.0.0.1");
	for(int i = 1; i < 2; i++){
		port = i + 5100;
		if(nodeadd(ip, port, 1024*10) == 0){
			cout << "该记录已经存在" << endl;
			break;
		}
	}
*/
/*
	char c[20];
	cout << "输入用户" << endl;
	while(1)
	{
		scanf("%s", c);
		if(strcmp(c, "0") == 0)
			break;
		if(useradd(c) != 1)
		{
			cout << "用户已经存在" << endl;
			continue;
		}
	}
	
	fseek(fp_user, 0, 0);
	userInfo user;
	while( fread(&user, sizeof(userInfo), 1, fp_user) == 1)
		cout << user.user << "    " << user.ip << ":" << user.port << endl;

	cout << "输入用户" << endl;
	while(1)
	{
		scanf("%s", c);
		if(strcmp(c, "0") == 0)
			break;
		if(userget(c, &user) != 1)
		{
			cout << "用户不存在" << endl;
			continue;
		}
		cout << user.user << "    " << user.ip << ":" << user.port << endl;
	}
	cout << "output:\n";

	fseek(fp_node, 0, 0);
	nodeInfo node2;
	while( fread(&node2, sizeof(node2), 1, fp_node) == 1)
		cout<< node2.ip << ":" << node2.port << "    " << node2.totalsize << "," << node2.freesize << "," << node2.userNum  << endl;
*/

