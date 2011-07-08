#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <dde.h>

#include "udialogx.h"
#include "udlgapp.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        //return initConversation();
    }

    int initConversation()
    {
        static BOOL fInInitiate = FALSE;
        char *szApplication;
        char *szTopic;
        ATOM atomApplication = *szApplication == 0 ?
            NULL     : GlobalAddAtom((LPSTR) szApplication);
        ATOM atomTopic = *szTopic == 0 ?
            NULL     : GlobalAddAtom((LPSTR) szTopic);

        fInInitiate = TRUE;
        ::SendMessage((HWND) HWND_BROADCAST,     // broadcasts message
                WM_DDE_INITIATE,               // initiates conversation
                (WPARAM) m_hDlg,               // handle to client DDE window
                MAKELONG(atomApplication,      // application-name atom
                    atomTopic));               // topic-name atom
        fInInitiate = FALSE;
        if (atomApplication != 0)
            GlobalDeleteAtom(atomApplication);
        if (atomTopic != 0)
            GlobalDeleteAtom(atomTopic);
        return 0;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);

