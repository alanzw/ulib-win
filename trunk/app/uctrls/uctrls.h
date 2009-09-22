/*
 * =====================================================================================
 *
 *       Filename:  usysadm.h
 *
 *    Description:  Class MyDialog
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:20:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */


#ifndef U_MYDIALOG_CTRLS_H
#define U_MYDIALOG_CTRLS_H

#include "udialogx.h"

#include "umsg.h"

#include "ubutton.h"
#include "ustatic.h"


#include "adt/uvector.h"

class UCtrlFactory
{
public:
    typedef huys::ADT::UVector<UControl *> UCtrls;

    UCtrlFactory()
    {}

    virtual ~UCtrlFactory()
    {
        UCtrls::const_iterator it;
        for( it = m_ctrls.begin(); it != m_ctrls.end(); ++it)
        {
            delete *it;
        }
    }

    virtual UControl * create(HWND hParent, UINT id, const TCHAR *lpTitle, LPRECT lpRect) = 0;
protected:
    UCtrls m_ctrls;
};

class UButtonFactory : public UCtrlFactory
{
public:
    virtual UControl * create(HWND hParent, UINT id, const TCHAR *lpTitle, LPRECT lpRect)
    {
        UControl *pCtrl = new UButton(hParent, id, ::GetModuleHandle(NULL));
        //pCtrl->setRect(lpRect);
        pCtrl->create();
        pCtrl->setWindowText(lpTitle);
        pCtrl->setPosition(lpRect);
        m_ctrls.push_back(pCtrl);
        return pCtrl;
    }
};

class UStaticFactory : public UCtrlFactory
{
public:
    virtual UControl * create(HWND hParent, UINT id, const TCHAR *lpTitle, LPRECT lpRect)
    {
        UControl *pCtrl = new UStatic(hParent, id, ::GetModuleHandle(NULL));
        //pCtrl->setRect(lpRect);
        pCtrl->create();
        pCtrl->setWindowText(lpTitle);
        pCtrl->setPosition(lpRect);
        m_ctrls.push_back(pCtrl);
        return pCtrl;
    }
};

using huys::UDialogBox;

class UDialogCtrls : public UDialogBox
{
public:
    UDialogCtrls(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pCtrlFactory(0)
    {}

    ~UDialogCtrls()
    {
        CHECK_PTR(m_pCtrlFactory);
    }

    BOOL onInit()
    {
        return TRUE;
    }

    BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        if (0 == m_pCtrlFactory)
        {
            m_pCtrlFactory = new UButtonFactory;
            RECT rc = {100, 100, 200, 200};
            m_pCtrlFactory->create(m_hDlg, 333, TEXT("Button"), &rc);
        }
        return FALSE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        if (0 == m_pCtrlFactory)
        {
            m_pCtrlFactory = new UStaticFactory;
            RECT rc = {100, 100, 200, 200};
            m_pCtrlFactory->create(m_hDlg, 333, TEXT("Static"), &rc);
        }
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch ( LOWORD(wParam) )
        {
        case 333:
            {
                showMsg(_T("xxx"), _T("info"), m_hDlg);
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UCtrlFactory *m_pCtrlFactory;
};

#endif // U_MYDIALOG_CTRLS_H

