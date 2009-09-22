#include <windows.h>
#include <tchar.h>

#include "ufileman.h"
#include "uwget.h"

struct OptionS {
    int nIndex;
    const char *sShortName;
    const char *sLongName;
    bool bHasParam;
    char *Params;
};

static OptionS wget_opts[] =
{
    { 0, "-o", "--output-file",   true, 0},
    { 1, "-a", "--append-output", true, 0},
    { -1, 0, 0, false, 0}
};

WgetInvoker :: WgetInvoker()
 : m_bRecursive(false),
   m_bProxyUsed(false),
   m_bContinued(false),
   m_bQuiet(false),
   m_bLogEnable(false)
{
}

WgetInvoker::~WgetInvoker()
{}

bool WgetInvoker::invoke()
{
    return true;
}

bool WgetInvoker::isExeFileOk() const
{
    // TODO: change to the desired ExeFile Path
    return UFileMan::isFileReady("wget.exe");
}

void WgetInvoker::setRecursive( bool bRecursive /*= true*/ )
{
    m_bRecursive = bRecursive;
}

void WgetInvoker::setProxyUsed( bool bProxyOn /*= true*/ )
{
    m_bProxyUsed = bProxyOn;
}

void WgetInvoker::setContinued( bool bContinued /*= true*/ )
{
    m_bContinued = bContinued;
}


