#ifndef U_MSG_H
#define U_MSG_H

#include "ulib.h"

// Message
void ULIB_API showMsg(const TCHAR *szMsg, const TCHAR *szTitle=_T("Info"), HWND hOwner=NULL);

//
int ULIB_API showMsgFormat(const TCHAR * szCaption, const TCHAR * szFormat, ...);

//
void ULIB_API showError(const TCHAR *msg);

//
void ULIB_API showErrorByNum(DWORD errorNum);

//
void ULIB_API getErrorString(DWORD errorNum, TCHAR *lpBuffer);

int ULIB_API LoadMessage(DWORD dwMsgId, PTSTR pszBuffer, UINT cchBuffer, ...);

void ULIB_API showCustomMsgBox(const TCHAR *szMsg, const TCHAR *szTitle=_T("Info"), HWND hOwner=NULL);

void ULIB_API showCustomInfoMsg(const TCHAR *szMsg, HWND hOwner);

//
/***********************************************************************
*
*  TimedMessageBox
*
*      The same as the standard MessageBox, except that TimedMessageBox
*      also accepts a timeout. If the user does not respond within the
*      specified timeout, the value 0 is returned instead of one of the
*      ID* values.
*
***********************************************************************/
int ULIB_API TimedMessageBox(HWND hwndOwner,
                    LPCTSTR pszMessage,
                    LPCTSTR pszTitle,
                    UINT flags,
                    DWORD dwTimeout);

#endif // U_MSG_H
