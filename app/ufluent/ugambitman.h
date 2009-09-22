/*
 * =====================================================================================
 *
 *       Filename:  ugambitman.h
 *
 *    Description:  class UGambitMan
 *
 *        Version:  1.0
 *        Created:  2009-8-5 0:43:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_GAMBIT_MAN_H
#define U_GAMBIT_MAN_H

class UGambitMan
{
public:
    enum Ugm_Error {
        UGME_EXCEED_ERROR,
        UGME_LOCKED
    };
public:
    UGambitMan();
    ~UGambitMan();

    //
    bool startExe();
    bool exitExe();

    //
    bool findWindow();

    //
    HWND getGambitWnd() const
    { return m_hGambitWnd; }
    //
    bool isExceedOk();

    //
    void setTargetWnd(HWND hWnd)
    { m_utParam.hWnd = hWnd;}

    //
    void sendCmdGambitStartup();
private:
    TCHAR m_sGambitDir[MAX_PATH];
    TCHAR m_sGambitArch[10];
    TCHAR m_sGambitBinDir[10];

    HWND m_hGambitStartup;
    HWND m_hGambitWnd;

    //
    TCHAR m_sStartupScriptFile[MAX_PATH];
private:
    UThreadParam m_utParam;
};

#define UGM_FINISHED_MSG _T("UGM_FINISHED_C80841D5-9C7B-4084-8059-7351A943173B")

//
#ifndef IMPLEMENT_MSG
#define IMPLEMENT_MSG(x) static UINT x = ::RegisterWindowMessage(x##_MSG)
#endif

//
IMPLEMENT_MSG(UGM_FINISHED);

#endif // U_GAMBIT_MAN_H

