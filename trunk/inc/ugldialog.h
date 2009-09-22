#ifndef U_GLDIALOG_H
#define U_GLDIALOG_H

#include "udialogx.h"

namespace huys
{

class ULIB_API UGLDialog : public UDialogBox
{
public:
    UGLDialog( HINSTANCE hInst,
               UINT nID,
               DLGPROC lpDialogFunc=UDialogBox::DefaultDlgProc,
               HWND hParent = NULL);
    virtual ~UGLDialog();
    //
    virtual BOOL create();

    virtual BOOL onCancel();
    virtual BOOL onDestroy();

    virtual BOOL initGL() { return TRUE;};
    virtual BOOL animate();
protected:
private:
    virtual BOOL go();

    HDC m_hdc;
    HGLRC m_hrc;

    BOOL m_bQuit;
};


}; // namespace huys

#endif // U_GLDIALOG_H

