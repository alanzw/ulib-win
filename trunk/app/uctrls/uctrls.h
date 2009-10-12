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

#endif // U_MYDIALOG_CTRLS_H

