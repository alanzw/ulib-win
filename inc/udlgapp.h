#ifndef U_DLGAPP_H
#define U_DLGAPP_H

//extern HINSTANCE g_hInst;

#define UDLGAPP(nResID, DlgProc) \
HINSTANCE g_hInst = 0; \
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow) \
{ \
    g_hInst = hInstance; \
    UDialogBox dialog(hInstance, nResID, DlgProc);  \
    dialog.create(); \
    return 0;\
}

#define UDLGAPP_T(TDialog, nResID) \
HINSTANCE g_hInst = 0; \
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow) \
{ \
    g_hInst = hInstance; \
    TDialog dialog(hInstance, nResID);  \
    dialog.create(); \
    return 0;\
}

#define UDLGAPPEx(nResID) \
    UDLGAPP_T(UDialogBox, nResID)

#define UDLGAPP_T0(TDialog) \
    HINSTANCE g_hInst = 0; \
    int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow) \
{ \
    g_hInst = hInstance; \
    TDialog dialog;  \
    dialog.create(); \
    return 0;\
}

#define BEGIN_DLGAPP \
HINSTANCE g_hInst = 0; \
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow) \
{ \
    g_hInst = hInstance;

#define END_DLGAPP \
    return 0;\
}

#endif // U_DLGAPP_H
