/*
 * =====================================================================================
 *
 *       Filename:  ufluentman.h
 *
 *    Description:  Class UFluentMan
 *
 *        Version:  1.0
 *        Created:  08/04/2009 09:33:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_FLUENT_MAN_H
#define U_FLUENT_MAN_H

class UFluentMan
{
public:
    enum Ufm_Error {
        UFME_PATH_ERROR,
        UFME_ARCH_ERROR,
        UFME_BIN_ERROR
    };

    enum Ufm_State {
        UFMS_NONE,
        UFMS_READY,
        UFMS_DONE
    };
public:
    UFluentMan();
    ~UFluentMan();

    //
    bool startExe(LPCTSTR lpFluentDir = "D:\\GS\\Fluent.Inc", LPCTSTR lpStartup = "fluent.scm");
    bool exitExe();

    //
    bool findWindow();

    //
    Ufm_State state();

    //
    bool sendCmd(LPCSTR sCmd);
    bool sendCmdChar(char c);
    bool sendCmdEnter();

    //
    bool getResult();

    //
    bool breakCalc();

    //
    bool showWnd(bool bVisible);

    //
    void setTargetWnd(HWND hWnd)
    { m_utParam.hWnd = hWnd;}

    //
    HWND getFluentWnd() const
    { return m_hFluentWnd; }

    //
    bool loadScript(LPCSTR sFileName);

    void addMenu();
    void removeMenu();
    void subclassWnd(HWND hMsgWnd);
    void focusWnd();

    void clearup();
    
    int fetchNewResult();
private:
    char m_sFluentDir[MAX_PATH];
    char m_sFluentArch[10];
    char m_sFluentBinDir[10];

    char m_sFluentVersion[10];

    HWND m_hFluentWnd;
    HWND m_hFluentEditCtrl;

    char m_buf[1024];

    static WNDPROC m_OriginProc;
    static LRESULT newWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    char m_sStartupScriptFile[MAX_PATH];
private:
    int getLineCount() const;
    int getLine(int nLineIndex);

    int getNewLines();

    int m_nLineCount;
    int m_nLineCountPrev;
private:
    UThreadParam m_utParam;

    HMENU m_hPopupMenu;
};

#define UFM_FINISHED_MSG _T("UFM_FINISHED_C1B96D9E-B9DC-4e04-8158-7B6776B099E9")
#define UFM_UPDATE_MSG _T("UFM_UPDATE_FA3EADB0-3E98-4011-AB31-BFB9005C7A77")
//
#ifndef IMPLEMENT_MSG
#define IMPLEMENT_MSG(x) static UINT x = ::RegisterWindowMessage(x##_MSG)
#endif

//
IMPLEMENT_MSG(UFM_FINISHED);
IMPLEMENT_MSG(UFM_UPDATE);

#endif // U_FLUENT_MAN_H

