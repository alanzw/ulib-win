/*
 * =====================================================================================
 *
 *       Filename:  umytree.h
 *
 *    Description:  Class MyTree
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


#ifndef U_MYTREE_H
#define U_MYTREE_H

#include "utreeview.h"

#include "adt/utable.h"

class UMyTreeCtrl : public UTreeView
{
public:
    enum EItemState {
        ISA_EXPANDED = 0x01,
        ISA_CHECKED  = 0x02,
        ISA_DISABLED = 0x04
    };

    enum EItemStyle {
        ISY_EDITABLE    = 0x01,
        ISY_CHECHEDNODE = 0x02,
        ISY_RADIONODE   = 0x04,
        ISY_SELECTABLE  = 0x08
    };

    typedef struct _tagItemRects
    {
        RECT rcLeft;
        RECT rcExpand;
        RECT rcCheck;
        RECT rcText;
    } ItemRects, *ItemRectsPtr;

    typedef huys::ADT::UTable<HTREEITEM, ItemRectsPtr> ItemRectsTable;
public:
	UMyTreeCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
	: UTreeView(hParent, nID, hInst)
	{}

	BOOL create()
	{
		BOOL ret = UTreeView::createEx(0, _T("SysTreeView32"));
		this->subclassProc();
		return ret;
	}


private:
    HTREEITEM      m_hEditNode;
    ItemRectsTable m_tItemRects;
    bool           m_bQuitEdits;
    COLORREF       m_xBackGroundColor;
};

#endif // U_MYTREE_H

