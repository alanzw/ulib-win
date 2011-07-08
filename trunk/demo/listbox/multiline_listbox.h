#ifndef U_MULTILINE_LISTBOX_H
#define U_MULTILINE_LISTBOX_H

#define MSG_UPDATEITEM WM_USER + 0x1001
#define ITEM_HEIGHT 20

class UMultiLineListBox : public UListBox
{
	struct _SubNodeInfo {
		
		
	};

public:
    UMultiLineListBox(HWND hParent, UINT nID, HINSTANCE hInst)
        : UListBox(hParent, nID, hInst)
    {
        m_dwStyles |= LBS_OWNERDRAWVARIABLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS;

        // Default Item Height: 36
        _nItemHeight = 36;
    }

    virtual BOOL create()
    {
        // Drag-and-Drop is supported
        return UListBox::create() && this->subclassProc();
    }

    virtual BOOL onMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
    {
		if (MSG_UPDATEITEM == nMessage)
		{
			return this->onUpdateItem(wParam, lParam);
		}
	
        return UListBox::onMessage(nMessage, wParam, lParam);
    }

    virtual BOOL onMeasureItem(WPARAM wParam, LPARAM lParam)
    {
        LPMEASUREITEMSTRUCT lpmis;
        lpmis = (LPMEASUREITEMSTRUCT) lParam;

        // Set the height of the list box items.
        lpmis->itemHeight = _nItemHeight;

        return TRUE;
    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpdis;
        lpdis = (LPDRAWITEMSTRUCT) lParam;

        USmartDC dc(lpdis->hDC);
        UMemDC memDC(dc);

        TEXTMETRIC tm;
        TCHAR tchBuffer[MAX_PATH] = {0};

        RECT rcBitmap;
        int y;

        // If there are no list box items, skip this message.
        if (lpdis->itemID == (unsigned int)-1)
        {
            return TRUE;
        }

        //SetBkMode(lpdis->hDC, TRANSPARENT);
        dc.setBKMode(TRANSPARENT);

        //_crOldBkColor = ::GetBkColor(lpdis->hDC);
        //_crOldTextColor = ::GetTextColor(lpdis->hDC);
        _crOldBkColor = dc.getBKColor();
        _crOldTextColor = dc.getTextColor();

        // Draw the bitmap and text for the list box item. Draw a
        // rectangle around the bitmap if it is selected.
        switch (lpdis->itemAction)
        {
        case ODA_SELECT:
            if (lpdis->itemState & ODS_SELECTED)
            {
                // Fill the item rect with the highlight blue color
                dc.setBKColor(::GetSysColor(COLOR_HIGHLIGHT));
                dc.extTextOut(0, 0, ETO_OPAQUE, &lpdis->rcItem, NULL, 0, NULL);

                // Set the color of the background of the text rect
                dc.setBKColor(::GetSysColor(COLOR_HIGHLIGHT));

                // Set the color of the text
                dc.setTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
            }
            else
            {
                // Fill the item rect with the highlight blue color
                dc.setBKColor(::GetSysColor(COLOR_BTNFACE));
                dc.extTextOut(0, 0, ETO_OPAQUE, &lpdis->rcItem, NULL, 0, NULL);
            }
        case ODA_DRAWENTIRE:

            // Display the text associated with the item.
            SendMessage(lpdis->hwndItem, LB_GETTEXT,
                lpdis->itemID, (LPARAM) tchBuffer);

            GetTextMetrics(dc, &tm);

            y = (lpdis->rcItem.bottom + lpdis->rcItem.top -
                tm.tmHeight) / 2;

            SetTextColor(lpdis->hDC, huys::xpblue);
            dc.textOut(16 + 6,
                        y,
                        tchBuffer,
                        strlen(tchBuffer));

            // Is the item selected?
            if (lpdis->itemState & ODS_SELECTED)
            {
                // Set RECT coordinates to surround only the
                // bitmap.
                rcBitmap.left = lpdis->rcItem.left;
                rcBitmap.top = lpdis->rcItem.top;
                //rcBitmap.right = lpdis->rcItem.left + XBITMAP;
                //rcBitmap.bottom = lpdis->rcItem.top + YBITMAP;
                rcBitmap.right = lpdis->rcItem.right;
                rcBitmap.bottom = lpdis->rcItem.bottom;
                // Draw a rectangle around bitmap to indicate
                // the selection.
                DrawFocusRect(lpdis->hDC, &rcBitmap);
            }
            break;

        case ODA_FOCUS:
            // Do not process focus changes. The focus caret
            // (outline rectangle) indicates the selection.
            // The IDOK button indicates the final
            // selection.
            break;
        }
        return TRUE;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        return (BOOL)(HBRUSH)::GetSysColorBrush(COLOR_BTNFACE);
    }

public:
    void setAllItemHeight(int n)
    {
        _nItemHeight = n;
    }
private:
	LRESULT onUpdateItem(WPARAM wParam, LPARAM lParam)
	{
		int nPreIndex = (int)wParam;
		int nCurIndex = (int)lParam;

		if(-1 != nPreIndex)
		{
			this->setItemHeight(ITEM_HEIGHT, nPreIndex);
		}
	
		if(-1 != nCurIndex)
		{
			int nItemCount = 1;
			//LISTBOXINFO* pListBox = m_sArray.at(nCurIndex);
			//ASSERT(pListBox);
			//nItemCount += static_cast<int>(pListBox->subArray.size());
			this->setItemHeight(ITEM_HEIGHT * nItemCount, nCurIndex);
		}

		this->invalidate(); // Update item
		return 0;
	}
private:
	int _nFocusIndex;
    int _nItemHeight;

    COLORREF _crOldTextColor;
    COLORREF _crOldBkColor;

};


#endif // U_MULTILINE_LISTBOX_H
