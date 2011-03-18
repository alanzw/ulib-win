#ifndef U_CUSTOM_LISTCTRL_H
#define U_CUSTOM_LISTCTRL_H

#include "ulistview.h"
#include "colors.h"
#include "uedit.h"
#include "ucombobox.h"
#include "adt/ustring.h"

#define WM_UPDATE_LVITEM WM_USER+122

class UCustomListView : public UListView
{
	typedef huys::ADT::UStringAnsi UString;
public:
    UCustomListView(HWND hParent, UINT nID, HINSTANCE hInst=::GetModuleHandle(NULL))
        : UListView(hParent, nID, hInst)
    {
		m_row = -1;
		m_col = -1;

		m_pEmEdit = NULL;

		m_sItemText.reserve(1024);
    }

    ~UCustomListView()
    {

    }

    BOOL create()
    {
        return  (UListView::create() && subclassProc());
    }

    BOOL doCustomDraw(LPNMLVCUSTOMDRAW lplvcd)
    {
        if (NULL == lplvcd)
        {
            return FALSE;
        }
        // 1
        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_NOTIFYITEMDRAW));
        }

        // 2
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_NOTIFYSUBITEMDRAW));
        }
        // 3
        else if (lplvcd->nmcd.dwDrawStage &  (CDDS_SUBITEM | CDDS_PREPAINT))
        {
            // row colors
            if (lplvcd->nmcd.dwItemSpec % 2 == 0)
                lplvcd->clrTextBk = huys::orange;
            else
                lplvcd->clrTextBk = huys::pink;

            // subitem 0
            if (lplvcd->iSubItem == 0)
                lplvcd->clrTextBk = RGB(0xA6, 0xCA, 0xF0);

            // subitem colors
            if (lplvcd->iSubItem % 3 == 0)
                lplvcd->clrText = huys::red;
            else
                lplvcd->clrText = huys::green;

            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_NEWFONT));
        }
        else
        {
            ::SetWindowLong(m_hParent, DWL_MSGRESULT, (LONG)(CDRF_DODEFAULT));
        }

        return TRUE;
    }

	class EmEdit : public UEdit
	{
	public:
		EmEdit(HWND hParent, UINT nResource, HINSTANCE hInst)
		: UEdit(hParent, nResource, hInst)
		{
		
		}
		
		EmEdit(UBaseWindow *pWndParent, UINT nID)
		: UEdit(pWndParent, nID) 
		{
		}

		// Redirect the Control Procedure
		BOOL create()
		{
			return UEdit::create() && this->subclassProc();
		}

		BOOL onKillFocus(WPARAM wParam, LPARAM lParam)
		{
			TCHAR buffer[1024];
			this->getWindowText(buffer, 1024);
			this->sendMsgParent(WM_UPDATE_LVITEM, (WPARAM)buffer);
			return FALSE;
		}

		BOOL onKeyDown(WPARAM wParam, LPARAM lParam)
		{
			if (wParam == VK_RETURN || wParam == VK_ESCAPE )
			{
				::SetFocus(m_hParent);
				onKillFocus(0,0);
				return FALSE;
			}
			return UEdit::onKeyDown(wParam, lParam);
		}

		BOOL onChar(WPARAM wParam, LPARAM lParam)
		{
			if (wParam == VK_RETURN )
			{
				::SetFocus(m_hParent);
				onKillFocus(0,0);
				return FALSE;
			}

			return UEdit::onChar(wParam, lParam);
		}  
	
	};
    
    class EmComboBox : UComboBox
    {
	public:
		EmComboBox(HWND hParent, UINT nResource, HINSTANCE hInst)
		: UComboBox(hParent, nResource, hInst)
		{
		
		}
		
		EmComboBox(UBaseWindow *pWndParent, UINT nID)
		: UComboBox(pWndParent, nID) 
		{
		}
        
        BOOL create()
		{
			return UComboBox::create() && this->subclassProc();
		}
        
    };

    BOOL onNotifyReflect(WPARAM wParam, LPARAM lParam)
    {
        if ((UINT)NM_CUSTOMDRAW == ((LPNMHDR)lParam)->code)
        {
            return doCustomDraw((LPNMLVCUSTOMDRAW) lParam);
        }
		
		if ((UINT)NM_CLICK == ((LPNMHDR)lParam)->code)
		{
			return doClicked((LPNMITEMACTIVATE)lParam);
		}

		if ((UINT)NM_DBLCLK == ((LPNMHDR)lParam)->code)
		{
			return doDBClicked((LPNMITEMACTIVATE)lParam);
		}
        return UListView::onNotifyReflect(wParam, lParam);
    }

	BOOL onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (WM_UPDATE_LVITEM == message)
		{
			LPCSTR str = (LPCSTR)wParam;
			this->setItemText(m_row, m_col, str);
			//endEdit();
			return TRUE;
		}

		return UListView::onMessage(message, wParam, lParam);
	}
//private:
	BOOL doDBClicked(LPNMITEMACTIVATE lpnia)
	{
		//showMsgFormat("info", "Double Clicked : %d %d", lpnia->iItem, lpnia->iSubItem);
		if (-1 != lpnia->iItem)
		{
			if (m_row == lpnia->iItem && m_col == lpnia->iSubItem)
			{
				return FALSE;
			}

		    m_row = lpnia->iItem;
		    m_col = lpnia->iSubItem;

		    this->getItemText(m_row, m_col, m_sItemText.getBuffer(), 1024);
			m_sItemText.update();

			startEdit();

			if (this->isItemChecked(lpnia->iItem))
			{
				//showMsg("checked!");
				this->setItemChecked(lpnia->iItem, FALSE);
			}
		}
		else
		{

		}
		
		return FALSE;
	}

	BOOL doClicked(LPNMITEMACTIVATE lpnia)
	{
		if (-1 != lpnia->iItem)
		{
			//showMsgFormat("info", "Clicked : %d %d", lpnia->iItem, lpnia->iSubItem);
			if (m_row != lpnia->iItem || m_col != lpnia->iSubItem)
			{
				endEdit();
			}

		}
		else
		{
			endEdit();
		}

		return FALSE;
	}

	void startEdit()
	{
		if(!m_pEmEdit) //If Edit box does not exist create one
		{
			m_pEmEdit = new EmEdit(m_hSelf, 1234, m_hInstance);
			huys::URectL rect;
			this->getSubItemRect(m_row, m_col, rect);
            
            if (0 == m_col)
            {
                rect.setRight(this->getColumnWidth(0));
            }
            
			m_pEmEdit->setRect(rect);
			m_pEmEdit->create();
			m_pEmEdit->setFont(this->getFont());
			m_pEmEdit->setWindowText(m_sItemText);
			m_pEmEdit->setFocus();
		}
		else  //If Edit box exist just move it
		{
			huys::URectL rect;
			this->getSubItemRect(m_row,m_col,rect);
			//m_pedit->SetWindowPos(NULL,rect.left,rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
			m_pEmEdit->setWindowText(m_sItemText);
			m_pEmEdit->setPosition(rect);
			//m_pedit->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height(),1);
			m_pEmEdit->setFocus();
		}
	}

	void endEdit()
	{ 	
		if (NULL != m_pEmEdit)
 		{
			updateText();
 			m_pEmEdit->hide();
			m_pEmEdit->destroy();
			delete m_pEmEdit;
 		}
 		m_pEmEdit = NULL;
		m_row = -1;
		m_col = -1;
	}

	void updateText()
	{
		TCHAR buffer[1024];
		m_pEmEdit->getWindowText(buffer, 1024);
		this->sendMsg(WM_UPDATE_LVITEM, (WPARAM)buffer);
	}
private:
	int m_row;
	int m_col;
	UString m_sItemText;

	EmEdit * m_pEmEdit;


};

#endif // U_CUSTOM_LISTCTRL_H
