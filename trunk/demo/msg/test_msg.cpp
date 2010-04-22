#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "umsg.h"
#include "uwinapp.h"
#include "ubasewindow.h"
#include "colors.h"
#include "ubutton.h"

#include "adt/uautoptr.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_BN_CBTMSGBOX = 11000,
        ID_BN_MSGBOX    = 11001,
        ID_BN_ERROR     = 11002,
        ID_BN_TIMEDMB   = 11003
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("MsgBox Demo"));
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_ULIB);

       m_pBnCBT = new UButton(this, ID_BN_CBTMSGBOX);
       m_pBnCBT->setPos(100, 50, 100, 50);
       m_pBnCBT->create();
       m_pBnCBT->setWindowText(_T("CBT MsgBox"));

       m_pBnMsg = new UButton(this, ID_BN_MSGBOX);
       m_pBnMsg->setPos(250, 50, 100, 50);
       m_pBnMsg->create();
       m_pBnMsg->setWindowText(_T("MsgBox"));

       m_pBnErr = new UButton(this, ID_BN_ERROR);
       m_pBnErr->setPos(400, 50, 100, 50);
       m_pBnErr->create();
       m_pBnErr->setWindowText(_T("Error"));

       m_pBnTime = new UButton(this, ID_BN_TIMEDMB);
       m_pBnTime->setPos(550, 50, 100, 50);
       m_pBnTime->create();
       m_pBnTime->setWindowText(_T("Time"));

       return UBaseWindow::onCreate();
   }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case ID_BN_CBTMSGBOX:
            {
            //
            CBTMessageBox(*this, "CBTMsgBox", "info", MB_OK);
            return FALSE;
            }
        case ID_BN_MSGBOX:
            {
            //
            showMsg(_T("hello!"));

            //
            showMsgFormat(_T("Formated Message"), _T("%d -- %d"), 22, 100);

            //
            showCustomMsgBox(_T("Customized Message"));

            //
            ColorMessageBox(NULL,
                "You can see the color!",
                "Color",
                MB_OK,
                huys::xpblue,
                huys::white);

            return FALSE;
            }
        case ID_BN_ERROR:
            {
            //
            showError(_T("Error:"));

            //
            showErrorByNum(22);
            return FALSE;
            }
        case ID_BN_TIMEDMB:
            {
            //
            UINT uiResult;

            //
            // Ask the user a question. Give the user five seconds to
            // answer the question.
            uiResult = TimedMessageBox( *this,
                                "Does a triangle have three sides?",
                                "Quiz",
                                MB_YESNO,
                                // NULL first parameter is important.
                                5000);

            switch (uiResult)
            {
            case IDYES:
                MessageBox( NULL,
                    "That's right!",
                    "Result",
                    MB_OK);
                break;

            case IDNO:
                MessageBox(NULL,
                     "Believe it or not, triangles "
                     "really do have three sides.",
                     "Result",
                     MB_OK);
                break;

            case -1:
                MessageBox(NULL,
                     "I sensed some hesitation there.  "
                     "The correct answer is Yes.",
                     "Result",
                     MB_OK);
                break;
            }
            return FALSE;
            }
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UButton> m_pBnCBT;
    huys::ADT::UAutoPtr<UButton> m_pBnMsg;
    huys::ADT::UAutoPtr<UButton> m_pBnErr;
    huys::ADT::UAutoPtr<UButton> m_pBnTime;
};


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}
