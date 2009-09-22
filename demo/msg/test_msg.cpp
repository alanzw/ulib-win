#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "umsg.h"
#include "udlgapp.h"

//HINSTANCE g_hInst = 0;


//int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
//{
//    g_hInst = hInstance;

BEGIN_DLGAPP
    //
    showMsg(_T("hello!"));

    //
    showMsgFormat(_T("formated message"), _T("%d -- %d"), 22, 100);

    //
    showError(_T("Error:"));

    //
    showErrorByNum(22);

    //
    showCustomMsgBox(_T("Customized Message"));
    
    //
    UINT uiResult;

    //
    // Ask the user a question. Give the user five seconds to
    // answer the question.
    uiResult = TimedMessageBox( NULL, 
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


END_DLGAPP

//    return 0;
//}

