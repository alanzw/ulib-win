#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

#include "expat.h"

using namespace std;

#define XML_MMSC_LISTEN_PORT      "MMSC_LISTEN_PORT"
#define XML_CLIENT_LISTEN_PORT    "CLIENT_LISTEN_PORT"

#define XML_MMSC_FROMADDR         "MMSC_FROMADDR"
#define XML_MMSC_IPADDR           "MMSC_IPADDR"
#define XML_MMSC_ROOT             "MMSC_ROOT"
#define XML_MMSC_PORT             "MMSC_PORT"
#define XML_MMSC_LOGIN_NAME       "MMSC_LOGIN_NAME"
#define XML_MMSC_LOGIN_PWD        "MMSC_LOGIN_PWD"
#define XML_MMSC_VASP_ID          "MMSC_VASP_ID"
#define XML_MMSC_VAS_ID           "MMSC_VAS_ID"
#define XML_MMS_SUBMIT_REPEATTIME "MMS_SUBMIT_REPEATTIME"

enum CONF_TYPE{EM_UNUSE = 0,
    EM_LSN_MMSC_PORT,EM_LSN_CLN_PORT,EM_SENDREPEAT,
    EM_MMSC_IP,EM_MMSC_PORT,EM_MMSC_ROOT,
    EM_FROM,EM_VASPID,EM_VASID,
    EM_AUTHNAME,EM_AUTHPWD
};

struct XML_MMSCConfInfo {
    int Depth;
    int Element;

    int MMSC_LISTEN_PORT;
    int CLIENT_LISTEN_PORT;

    char MMSC_FROMADDR[50];
    char MMSC_IPADDR[16];
    char MMSC_ROOT[255];
    int MMSC_PORT;
    char MMSC_LOGIN_NAME[255];
    char MMSC_LOGIN_PWD[255];
    char MMSC_VASP_ID[255];
    char MMSC_VAS_ID[255];
    int MMS_SUBMIT_REPEATTIME;

};


static int GetID(const char*name){
    if(!stricmp(name,XML_MMSC_LISTEN_PORT))    return EM_LSN_MMSC_PORT;
    if(!stricmp(name,XML_CLIENT_LISTEN_PORT))  return EM_LSN_CLN_PORT;

    if(!stricmp(name,XML_MMSC_FROMADDR))       return EM_FROM;
    if(!stricmp(name,XML_MMSC_IPADDR))         return EM_MMSC_IP;
    if(!stricmp(name,XML_MMSC_ROOT))           return EM_MMSC_ROOT;
    if(!stricmp(name,XML_MMSC_PORT))           return EM_MMSC_PORT;
    if(!stricmp(name,XML_MMSC_LOGIN_NAME))     return EM_AUTHNAME;
    if(!stricmp(name,XML_MMSC_LOGIN_PWD))      return EM_AUTHPWD;
    if(!stricmp(name,XML_MMSC_VASP_ID))        return EM_VASPID;
    if(!stricmp(name,XML_MMSC_VAS_ID))         return EM_VASID;
    if(!stricmp(name,XML_MMS_SUBMIT_REPEATTIME))return EM_SENDREPEAT;

    return EM_UNUSE;
}

static int SetElementValue(XML_MMSCConfInfo & Conf, const char * Value)
{
    bool HasQoute = false;
    const char * pstart = strchr(Value,'"');
    const char * pend;

    if (!pstart)
    {
        pstart  = Value;
        pend    = pstart;
    }
    else
    {
        pend = strchr( ++ pstart,'"');
        if ( !pend ) return -1;
        HasQoute    = true;
    }

    int len = pend - pstart;

    switch (Conf.Element)
    {
    case EM_LSN_MMSC_PORT:
        Conf.MMSC_LISTEN_PORT    = atoi(pstart);
        break;
    case EM_LSN_CLN_PORT:
        Conf.CLIENT_LISTEN_PORT  = atoi(pstart);
        break;
    case EM_SENDREPEAT:
        Conf.MMS_SUBMIT_REPEATTIME = atoi(pstart);
        break;
    case EM_MMSC_IP:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_IPADDR,pstart,
            len>sizeof(Conf.MMSC_IPADDR) ? sizeof(Conf.MMSC_IPADDR) : len);
        break;
    case EM_MMSC_PORT:
        Conf.MMSC_PORT  = atoi(pstart);
        break;
    case EM_MMSC_ROOT:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_ROOT,pstart,
            len>sizeof(Conf.MMSC_ROOT) ? sizeof(Conf.MMSC_ROOT):len);
        break;
    case EM_FROM:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_FROMADDR,pstart,
            len>sizeof(Conf.MMSC_FROMADDR)? sizeof(Conf.MMSC_FROMADDR):len);
        break;
    case EM_VASPID:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_VASP_ID,pstart,
            len>sizeof(Conf.MMSC_VASP_ID)? sizeof(Conf.MMSC_VASP_ID):len);
        break;
    case EM_VASID:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_VAS_ID,pstart,
            len>sizeof(Conf.MMSC_VAS_ID)? sizeof(Conf.MMSC_VAS_ID):len);
        break;
    case EM_AUTHNAME:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_LOGIN_NAME,pstart,
            len>sizeof(Conf.MMSC_LOGIN_NAME)? sizeof(Conf.MMSC_LOGIN_NAME):len);
        break;
    case EM_AUTHPWD:
        if( !HasQoute) return -1;
        strncpy(Conf.MMSC_LOGIN_PWD,pstart,
            len>sizeof(Conf.MMSC_LOGIN_PWD)? sizeof(Conf.MMSC_LOGIN_PWD):len);
        break;
    default:
        break;
    };

    return 0;
}

static void XMLCALL xmlstart(void *data, const char *el, const char **attr)
{
    XML_MMSCConfInfo * pmmscinf = (XML_MMSCConfInfo *) data;

    for (int i = 0; attr[i]; i++)
    {
        pmmscinf->Element = GetID(attr[i]);
        SetElementValue(*pmmscinf,attr[i+1]);
    }

    pmmscinf->Element    = GetID(el);
    pmmscinf->Depth ++;
}

static void XMLCALL xmlend(void *data, const char *el)
{
    ((XML_MMSCConfInfo*)data)->Element = EM_UNUSE;
    ((XML_MMSCConfInfo*)data)->Depth --;
}

static void XMLCALL parsedata(void *userData,const XML_Char *s,int len)
{
    string str;
    str.assign(s, len);
    SetElementValue(*(XML_MMSCConfInfo*)userData,str.c_str());
}

static int ParseMMSCConf( XML_MMSCConfInfo& Conf, const char * FileName)
{
    memset(&Conf, 0, sizeof(XML_MMSCConfInfo));

    ifstream ifs(FileName, ios::in|ios::binary);
    
    if(!ifs) return -1;

    char* buf;
    int len;
    ifs.seekg(0,ios::end);
    len = ifs.tellg();
    ifs.seekg(0,ios::beg);

    buf = new char[len];
    if(buf)
    ifs.read(buf,len);
    ifs.close();
    if(!buf)
    return -1;

    int done    = 0;
    int err     = 0;

    XML_Parser parser = XML_ParserCreate(NULL);

    if (!parser)
    {
        cerr << "Couldn't allocate memory for parser" << endl;
        return -1;
    }

    XML_SetElementHandler(parser, xmlstart, xmlend);
    XML_SetUserData(parser, &Conf);
    XML_SetCharacterDataHandler(parser, parsedata);

    if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR)
    {
        cerr << XML_ErrorString(XML_GetErrorCode(parser)) << " at line "
             << XML_GetCurrentLineNumber(parser) << endl;
        err = 1;
    }

    ifs.close();
    XML_ParserFree(parser);
    return err;
}

static void Serialize(const XML_MMSCConfInfo& Conf)
{
    printf("XML_MMSCConfInfo is:\n"
    "\tDepth:%d\tElement:%d\n"
    "\tMSC_LISTEN_PORT:%d\tCLIENT_LISTEN_PORT:%d\n"
    "\tMMSC_FROMADDR:[%s]  MMSC_IPADDR:[%s]  tMMSC_PORT:%d\n"
    "\tMMSC_ROOT:[%s]\tMSC_LOGIN_NAME:[%s]\tMMSC_LOGIN_PWD:[%s]\n"
    "\tMMSC_VASP_ID:[%s]\tMMSC_VAS_ID:[%s]\tMMS_SUBMIT_REPEATTIME:%d\n",
    Conf.Depth,
    Conf.Element,
    Conf.MMSC_LISTEN_PORT,
    Conf.CLIENT_LISTEN_PORT,
    Conf.MMSC_FROMADDR,
    Conf.MMSC_IPADDR,
    Conf.MMSC_PORT,
    Conf.MMSC_ROOT,
    Conf.MMSC_LOGIN_NAME,
    Conf.MMSC_LOGIN_PWD,
    Conf.MMSC_VASP_ID,
    Conf.MMSC_VAS_ID,
    Conf.MMS_SUBMIT_REPEATTIME);
};


int main()
{
    XML_MMSCConfInfo conf;
    ParseMMSCConf(conf,"config.xml");
    Serialize(Conf);
    return 0;
}
