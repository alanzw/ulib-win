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

#ifndef U_MYDIALOG_CTRLBAR_H
#define U_MYDIALOG_CTRLBAR_H


#include "ucontrol.h"
#include "colors.h"
//#include "ubutton.h"
//#include "ustatic.h"


//#include "adt/uvector.h"

class UCtrlBar : public UControl
{
public:
    //typedef huys::ADT::UVector<UControl *> UCtrls;

    UCtrlBar()
    {}

    virtual ~UCtrlBar()
    {
    }

	int getCount() const
	{
		return m_nCount;
	}



    //virtual UControl * create(HWND hParent, UINT id, const TCHAR *lpTitle, LPRECT lpRect) = 0;
protected:
    //UCtrls m_ctrls;
	int m_nCount;
};

#endif // U_MYDIALOG_CTRLS_H

