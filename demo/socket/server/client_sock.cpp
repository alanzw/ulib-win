#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <process.h>

using namespace std;

#define MAINIP "10.15.10.19"
#define MAINPORT    5100

#define BLOCKSIZE 1024
#define USERNAME_LEN    20
#define    FILENAME_LEN    64
#define    LISTLEN        5




#define USERADD 1
#define USERGET    2
#define USERDEL    3
#define GETNODE    4
#define USERINFOGET 5
#define FILEINFOADD 6
#define STOREBLOCK 7
#define GETDATA        8

typedef int CMD;

typedef struct
{
    char    user[20];
    char    ip[16];
    int        port;
}userInfo;

typedef struct
{
    int blockNO;
    char ip[16];
    int port;
}blockInfo;

typedef struct
{
    char    ip[16];
    int        port;
}nodeaddr;

typedef struct
{
    nodeaddr *e;
    int length;
    int size;
}nodeaddrList;


typedef struct
{
    char filename[FILENAME_LEN];
    int    blockNO;
    char ip[16];
    int port;
}fileInfo;


typedef struct
{
    fileInfo *e;
    int length;
    int size;
}fileInfoList;

typedef struct
{
    char buf[BLOCKSIZE];
    int    datalen;
    int    blockNO;
}data;

typedef struct
{
    data *e;
    int    length;
    int size;
}datalist;

typedef struct
{
    char username[USERNAME_LEN];
    char filename[FILENAME_LEN];
    char ip[16];
    int port;
    int blockNO;
    char data[BLOCKSIZE];
    int datalen;
}threadelem;

typedef struct
{
    threadelem *e;
    int length;
    int size;
}threadelemlist;



typedef struct
{
    HANDLE *e;
    int length;
    int size;
}threadlist;

typedef struct
{
    char username[USERNAME_LEN];
    fileInfo finfo;
}threadelem_r;

typedef struct
{
    threadelem_r *e;
    int length;
    int size;
}threadelemlist_r;


FILE *fp_user;

_off_t NUM = 0;

int getnode(nodeaddrList *L, int num);

int initFInfolist(fileInfoList *L)
{
    L->e = (fileInfo *) malloc (LISTLEN * sizeof(fileInfo));
    if(L->e == NULL)
    {
        printf("初始化表，分配内存失败");
        return 0;
    }
    L->length = 0;
    L->size = LISTLEN;
    return 1;
}

int addFInfolist(fileInfoList *L, fileInfo e)
{
    if(L->length  >= L->size)
    {
        fileInfo *newbase = (fileInfo *) realloc (L->e, (L->size + LISTLEN)*sizeof(fileInfo));
        if(newbase == NULL)
        {
            printf("向表添加数据，增加内存空间失败");
            return 0;
        }
        L->e = newbase;
        L->size += LISTLEN;
    }
    L->e[L->length] = e;
    ++ L->length;
    return 0;
}

int destroyFInfolist(fileInfoList *L)
{
    L->length = 0;
    L->size = 0;
    free(L->e);
    return 1;
}


int initnodelist(nodeaddrList *L)
{
    L->e = (nodeaddr *) malloc (LISTLEN * sizeof(nodeaddr));
    if(L->e == NULL)
    {
        printf("初始化表，分配内存失败");
        return 0;
    }
    L->length = 0;
    L->size = LISTLEN;
    return 1;
}

int addnodelist(nodeaddrList *L, nodeaddr e)
{
    if(L->length  >= L->size)
    {
        nodeaddr *newbase = (nodeaddr *) realloc (L->e, (L->size + LISTLEN)*sizeof(nodeaddr));
        if(newbase == NULL)
        {
            printf("向表添加数据，增加内存空间失败");
            return 0;
        }
        L->e = newbase;
        L->size += LISTLEN;
    }
    L->e[L->length] = e;
    ++ L->length;
    return 0;
}

int destroynodelist(nodeaddrList *L)
{
    L->length = 0;
    L->size = 0;
    free(L->e);
    return 1;
}


int initdatalist(datalist *L)
{
    L->e = (data *)malloc(BLOCKSIZE * sizeof(data));
    L->length = 0;
    L->size = BLOCKSIZE;
    return 0;
}
int adddatalist(datalist *L, data e)
{
    if(L->length >= L->size)
    {
        data *newbase = (data *) realloc (L->e, (L->size + BLOCKSIZE)*sizeof(data));
        if(newbase == NULL)
        {
            printf("向表添加数据，增加内存空间失败");
            return 0;
        }
        L->e = newbase;
        L->size += BLOCKSIZE;
    }
    L->e[L->length] = e;
    ++ L->length;
    return 0;
}

int destorydatalist(datalist *L)
{
    L->length = 0;
    L->size = 0;
    free(L->e);
    return 1;
}



int initthreadelemlist(threadelemlist *L)
{
    L->e = (threadelem *)malloc(LISTLEN * sizeof(threadelem));
    L->length = 0;
    L->size = LISTLEN;
    return 0;
}
int addthreadelemlist(threadelemlist *L, threadelem e)
{
    if(L->length  >= L->size)
    {
        threadelem *newbase = (threadelem *) realloc (L->e, (L->size + LISTLEN)*sizeof(threadelem));
        if(newbase == NULL)
        {
            printf("向表添加数据，增加内存空间失败");
            return 0;
        }
        L->e = newbase;
        L->size += LISTLEN;
    }
    L->e[L->length] = e;
    ++ L->length;
    return 0;
}
int destorythreadelemlist(threadelemlist *L)
{
    L->length = 0;
    L->size = 0;
    free(L->e);
    return 1;
}



int initthreadlist(threadlist *L)
{
    L->e = (HANDLE *)malloc(LISTLEN * sizeof(HANDLE));
    L->length = 0;
    L->size = LISTLEN;
    return 0;
}
int addthreadlist(threadlist *L, HANDLE e)
{
    if(L->length  >= L->size)
    {
        HANDLE *newbase = (HANDLE *) realloc (L->e, (L->size + LISTLEN)*sizeof(HANDLE));
        if(newbase == NULL)
        {
            printf("向表添加数据，增加内存空间失败");
            return 0;
        }
        L->e = newbase;
        L->size += LISTLEN;
    }
    L->e[L->length] = e;
    ++ L->length;
    return 0;
}
int destorythreadlist(threadlist *L)
{
    L->length = 0;
    L->size = 0;
    free(L->e);
    return 1;
}


int initthreadelemlist_r(threadelemlist_r *L)
{
    L->e = (threadelem_r *)malloc(LISTLEN * sizeof(threadelem));
    L->length = 0;
    L->size = LISTLEN;
    return 0;
}
int addthreadelemlist_r(threadelemlist_r *L, threadelem_r e)
{
    if(L->length >= L->size)
    {
        threadelem_r *newbase = (threadelem_r *) realloc (L->e, (L->size + LISTLEN)*sizeof(threadelem_r));
        if(newbase == NULL)
        {
            printf("向表添加数据，增加内存空间失败");
            return 0;
        }
        L->e = newbase;
        L->size += LISTLEN;
    }
    L->e[L->length] = e;
    ++ L->length;
    return 0;
}
int destorythreadelemlist_r(threadelemlist_r *L)
{
    L->length = 0;
    L->size = 0;
    free(L->e);
    return 1;
}

int filesearch(char* filename, FILE *fp)
{
    fileInfo ftmp;
    fseek(fp, 0, 0);
    while( fread(&ftmp, sizeof(fileInfo), 1, fp) == 1)
        if(strcmp(ftmp.filename, filename) == 0)
            return 0;
    return 1;
}
int filedel(char* filename, FILE *fp)
{
    fileInfo ftmp;
    fseek(fp, 0, 0);
    while(!feof(fp))
    {
        fread(&ftmp, sizeof(fileInfo), 1, fp);
        if(strcmp(ftmp.filename, filename) == 0)
        {
            strcpy(ftmp.filename, "0");
            fseek(fp, ftell(fp)-sizeof(fileInfo),0);
            fwrite(&ftmp, sizeof(fileInfo), 1, fp);
            fseek(fp,ftell(fp), 0);
        }
    }
    return 1;
}

int fileadd(fileInfoList *L, FILE * fp)
{
    char filename[20];
    fileInfo file;
    strcpy(filename, L->e[0].filename);
    if(filesearch(filename, fp) == 0)
        filedel(filename,fp);
    fseek(fp, 0, SEEK_END);
    for(int i = 0; i < L->length; i++)
    {
        file = L->e[i];
        fwrite(&file, sizeof(fileInfo), 1, fp);
    }
    return 1;
}
int fileget(char* filename, fileInfoList *L, FILE *fp)
{
    fileInfo file;
    fseek(fp, 0, 0);
    while(fread(&file, sizeof(fileInfo), 1, fp) == 1)
        if(strcmp(filename, file.filename) == 0)
            addFInfolist(L, file);
    return 1;
}


int filecut(datalist *L, FILE *fp) //文件分块
{

    char c;
    data d;
    memset(&d, 0, sizeof(data));
    d.blockNO = 0;
    d.datalen = BLOCKSIZE;
    int i = 0;
    fseek(fp, 0, 0);
    while(!feof(fp))
    {
        ++d.blockNO;
        d.datalen = BLOCKSIZE;
        for(i = 0; i < BLOCKSIZE; i ++)
        {
            c = fgetc(fp);
            d.buf[i] = c;
        }
        if(feof(fp))
        {
            d.datalen = i;
            adddatalist(L, d);
            break;
        }
        c = fgetc(fp);
        fseek(fp, -1L, 1);
        while(1)
        {
            if(c<'0'&& c> '9')
                break;
            if(d.buf[i-1]<'0'|| d.buf[i-1]> '9')
                break;
            else
            {
                --i;
                --d.datalen;
                fseek(fp, -1L, 1);
            }
        }

        adddatalist(L, d);
    }
    return 1;

}

int blockdatalist(datalist *L,threadelemlist *T, char* username, char* filename) //将数据块添加到顺序表在
{
    nodeaddrList N;
    initnodelist(&N);
    getnode(&N, L->length); //从主节点分配节点地址
    threadelem e;
    for(int i = 0; i < L->length; i++)
    {
        memset(&e, 0, sizeof(threadelem));
        strcpy(e.username, username);
        strcpy(e.filename, filename);
        strcpy(e.ip, N.e[i].ip);
        e.port = N.e[i].port;
        e.blockNO = L->e[i].blockNO;
        e.datalen = L->e[i].datalen;
        memcpy(e.data, L->e[i].buf, L->e[i].datalen);
        addthreadelemlist(T, e);
    }
    return 1;
}


int creatconnect(char* ip, int port, SOCKET * csock)
{


    struct sockaddr_in    csockaddr;
    int    csockaddr_len = sizeof(struct sockaddr_in);

    csockaddr.sin_family = AF_INET;
    csockaddr.sin_port = htons(port);
    csockaddr.sin_addr.s_addr = inet_addr(ip);

    if( (connect(*csock, (struct sockaddr *)&csockaddr, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
    {
        cout << "链接失败:" << WSAGetLastError() << endl;
        return 0;
    }

    return 1;
}


int userregister(char* username) //用户注册成功返回1，失败返回0；
{
    WSADATA    wsa;
    SOCKET    csock;

    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    csock = socket(AF_INET, SOCK_STREAM, 0);

    CMD cmd;
    if(creatconnect(MAINIP, MAINPORT, &csock) == 0)
    {
        cout << "链接主服务器失败" << endl;
        return 0;
    }
    cmd = USERADD;
    int result;
    send(csock, (char *)&cmd, sizeof(CMD), 0);

    send(csock, username, USERNAME_LEN, 0);
    recv(csock, (char *)&result, sizeof(int), 0);

    if(result == 0)
    {
        cout << "注册失败" << endl;
        closesocket(csock);
        WSACleanup();
        return 0;
    }
    closesocket(csock);
    WSACleanup();
    return 1;
}


int getuserfile(userInfo user, FILE *fp)
{
    WSADATA    wsa;
    SOCKET    blocksock;

    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    blocksock = socket(AF_INET, SOCK_STREAM, 0);
    if(creatconnect(user.ip, user.port, &blocksock) == 0)
    {
        cout << "链接节点服务器失败" << endl;
        closesocket(blocksock);
        WSACleanup();
        return 0;
    }

    //--------------------------
    //从节点获取用户文件信息文件
    //--------------------------
    CMD  cmd = USERINFOGET;
    send(blocksock, (char *)& cmd, sizeof(CMD), 0); //发送获取用户信息文件的命令
    send(blocksock, user.user, USERNAME_LEN, 0); //发送用户名
    _off_t    filelen;
    recv(blocksock, (char *)& filelen, sizeof(_off_t), 0); //接收用户信息文件的大下小
/*
    FILE    *fp_tmp;
    //fp_tmp = NULL;
    if( (fp_tmp = fopen("tmp", "wb+")) == NULL)
    {
        cout << "创建临时文件失败" << endl;
        closesocket(blocksock);
        WSACleanup();
        return 0;
    }//写入临时文件。
*/
    cout << "filelen:" << filelen << endl;
    if(filelen > 0) //如果用户信息文件存在且不为空，则获取文件内容
    {
        char* buf = (char *) malloc (filelen);
        cout << "a1" <<endl;
        recv(blocksock, buf, filelen, 0);    //接收文件内容
                cout << "a2" <<endl;
        fwrite(buf, 1, filelen, fp);
                cout << "a3" <<endl;
        fseek(fp, 0, 0);
        free(buf);
    }
    closesocket(blocksock);
    WSACleanup();
    return 1;
}

int    getuseraddr(userInfo *user)
{

    WSADATA    wsa;
    SOCKET    csock;
    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    csock = socket(AF_INET, SOCK_STREAM, 0);

    CMD cmd;
    if(creatconnect(MAINIP, MAINPORT, &csock) == 0)
    {
        cout << "链接主服务器失败" << endl;
        closesocket(csock);
        WSACleanup();
        return 0;
    }
    cmd = USERGET;
    send(csock, (char *)&cmd, sizeof(CMD), 0);
    cout << "getuseraddr  user:" << user->user << endl;
    send(csock, user->user, USERNAME_LEN, 0);
    recv(csock, (char *)user, sizeof(userInfo), 0);

    closesocket(csock);
    WSACleanup();
    return 1;
}

int    userlogin(char* username, userInfo *user, FILE *fp) //用户登录,返回值：1成功，0用户还没有文件，-1该用户还未注册，-2获取用户信息文件存放地址失败
{
    //userInfo user;
    strcpy(user->user,username);
    int r = 1;
    if(getuseraddr(user) == 0)
    {
        cout << "从主服务器获取用户信息文件存放地址失败" << endl;
        r = -2;
    }
    if(strcmp(user->user, "0") == 0)
    {
        cout << "没有该用户" << endl;
        r = -1;
    }
    cout << user->user << "  " << user->ip << ":"<< user->port << endl;
    if( getuserfile(*user, fp) == NULL)
    {
        cout << "从节点获取用户信息文件失败:该用户还没有文件" << endl;
        r = 0;
    }
    cout << "over" << endl;
    return r;
}



int getnode(nodeaddrList *L, int num) //从主节点分配节点地址
{

    WSADATA    wsa;
    SOCKET    csock;
    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    csock = socket(AF_INET, SOCK_STREAM, 0);

    CMD cmd;
    if(creatconnect(MAINIP, MAINPORT, &csock) == 0)
    {
        cout << "链接主服务器失败" << endl;
        closesocket(csock);
        WSACleanup();
        return 0;
    }
    cmd = GETNODE;
    nodeaddr node;
    send(csock, (char *)&cmd, sizeof(CMD), 0);
    send(csock, (char *)&num, sizeof(int), 0);
    for(int i = 0; i < num; i++)
    {
        recv(csock, (char *)&node, sizeof(node), 0);
        cout << "int:" << node.ip << ":" << node.port << endl;
        addnodelist(L, node);
    }

    closesocket(csock);
    WSACleanup();
    return 1;
}

int userinfoadd(fileInfoList *L, userInfo user,FILE *fp_tmp)//用户信息文件中添加一个文件信息
{
    cout << "userinfoadd:" << L->length << endl;
    WSADATA    wsa;
    SOCKET    blocksock;

    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    blocksock = socket(AF_INET, SOCK_STREAM, 0);
    if(creatconnect(user.ip, user.port, &blocksock) == 0)
    {
        cout << "链接节点服务器失败" << endl;
        closesocket(blocksock);
        WSACleanup();
        return 0;
    }

    CMD cmd = FILEINFOADD;
    char filename[FILENAME_LEN];
    strcpy(filename, L->e[0].filename);
    int listlen = L->length;
    blockInfo btmp;
    send(blocksock, (char *)&cmd, sizeof(CMD), 0); //发送添加文件信息命令
    send(blocksock, user.user, USERNAME_LEN, 0);    //发送用户名
    send(blocksock, filename, FILENAME_LEN, 0);    //发送文件名
    send(blocksock, (char *)&listlen, sizeof(int), 0);    //发送表长度
    for(int i = 0; i < listlen; i++)
    {
        strcpy(btmp.ip, L->e[i].ip);
        btmp.port = L->e[i].port;
        btmp.blockNO = L->e[i].blockNO;
        send(blocksock, (char *)& btmp, sizeof(blockInfo), 0);

        cout << user.user << ":" << filename << "  " << btmp.blockNO << "  " << btmp.ip << ":" << btmp.port << endl;
    }
    int result = 0;

    recv(blocksock, (char *)&result, sizeof(int), 0);

    if(result != 1)
    {
        cout << "在节点上添加文件信息失败" << endl;
        return 0;
    }
        cout << "1" <<endl;
    fileadd(L, fp_tmp);//将文件信息添加到本地临时文件；
        cout << "2" << endl;
    return 1;

}

unsigned _stdcall storedatathread(LPVOID p)
{
    threadelem *t = (threadelem *)p;

    WSADATA    wsa;
    SOCKET    csock;
    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    csock = socket(AF_INET, SOCK_STREAM, 0);

    CMD cmd;
    if(creatconnect(t->ip, t->port, &csock) == 0)
    {
        cout << "链接节点服务器失败" << endl;
        closesocket(csock);
        WSACleanup();
        return 0;
    }
    cmd = STOREBLOCK;
    send(csock, (char *)&cmd, sizeof(CMD), 0);
    send(csock, t->username, USERNAME_LEN, 0);
    send(csock, t->filename, FILENAME_LEN, 0);
    send(csock, (char *)&(t->blockNO), sizeof(int), 0);
    send(csock, (char *)&(t->datalen),sizeof(int), 0);
    send(csock, t->data, t->datalen, 0);
    cout << "datalen:--" << t->datalen << endl;

    closesocket(csock);
    WSACleanup();
    return 1;
}


unsigned _stdcall getdatathread(LPVOID p)//获取计算结果
{
    threadelem_r *t = (threadelem_r *)p;

    WSADATA    wsa;
    SOCKET    csock;
    int ret = 0;
    ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(ret != 0)
    {
        cout << "Init WinSock failed:"  << ret << endl;
        return 0;
    }
    csock = socket(AF_INET, SOCK_STREAM, 0);

    CMD cmd;
    if(creatconnect(t->finfo.ip, t->finfo.port, &csock) == 0)
    {
        cout << "链接节点服务器失败" << endl;
        closesocket(csock);
        WSACleanup();
        return 0;
    }
    cmd = GETDATA;
    _off_t result = 0;

    send(csock, (char *)&cmd, sizeof(CMD), 0);
    send(csock, t->username, USERNAME_LEN, 0);
    send(csock, t->finfo.filename, FILENAME_LEN, 0);
    send(csock, (char *)&(t->finfo.blockNO), sizeof(int), 0);

    recv(csock, (char *)&result, sizeof(_off_t), 0);

    NUM += result;


    closesocket(csock);
    WSACleanup();
    return 1;
}

int cwrite(userInfo user, char* filename, FILE *fp, FILE *fp_tmp)
{
    cout << "In cwrite " << endl;
    datalist D;
    threadelemlist T;
    threadlist H;
    initdatalist(&D);
    initthreadelemlist(&T);
    initthreadlist(&H);

    filecut(&D, fp);
    blockdatalist(&D, &T, user.user, filename);


    HANDLE hd;
    for(int i = 0; i < T.length; i ++)
    {
        hd =(HANDLE) _beginthreadex(NULL, 0, &storedatathread, &(T.e[i]), 0, NULL);
        addthreadlist(&H,hd);
    }
    WaitForMultipleObjects(H.length, H.e, true, INFINITE);


    fileInfo finfo;
    fileInfoList F;
    initFInfolist(&F);
    for(int i = 0; i < T.length; i++)
    {
        strcpy(finfo.filename, filename);
        strcpy(finfo.ip, T.e[i].ip);
        finfo.port = T.e[i].port;
        finfo.blockNO = T.e[i].blockNO;
        addFInfolist(&F, finfo);
    }

    Sleep(100);
    userinfoadd(&F,user, fp_tmp);



    destroyFInfolist(&F);
    destorydatalist(&D);
    destorythreadelemlist(&T);
    destorythreadlist(&H);
    return 1;
}

int getdata(char* username, char* filename, FILE *fp_tmp)//获取计算结果
{
    fileInfo file;
    fileInfoList L;
    threadlist H;
    threadelemlist_r T;

    initFInfolist(&L);
    initthreadlist(&H);
    initthreadelemlist_r(&T);

    fileget(filename, &L, fp_tmp);
    threadelem_r t;
    for(int i = 0; i < L.length; i++)
    {
        strcpy(t.username, username);
        strcpy(t.finfo.filename, L.e[i].filename);
        strcpy(t.finfo.ip, L.e[i].ip);
        t.finfo.port = L.e[i].port;
        t.finfo.blockNO = L.e[i].blockNO;
        addthreadelemlist_r(&T, t);
    }
    HANDLE hd;
    for(int i = 0; i < T.length; i++)
    {
        hd =(HANDLE) _beginthreadex(NULL, 0, &getdatathread, &(T.e[i]), 0, NULL);
        addthreadlist(&H,hd);
    }
    WaitForMultipleObjects(H.length, H.e, true, INFINITE);


    destroyFInfolist(&L);
    destorythreadelemlist_r(&T);
    destorythreadlist(&H);

    return 1;
}




int main()
{

    return 0;
}
