#ifndef U_CONTACT_LISTBOX_H
#define U_CONTACT_LISTBOX_H

#include "ulistbox.h"
#include "colors.h"

#define BUFFER MAX_PATH
#define XBITMAP 80
#define YBITMAP 20

class UContactListBox : public UListBox
{
public:
    UContactListBox(HWND hParent, UINT nID, HINSTANCE hInst);

    virtual BOOL create();

    void addItem(LPCTSTR str, UPicture *ppic);

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam);

    virtual BOOL onMeasureItem(WPARAM wParam, LPARAM lParam);

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);

        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }

    //BOOL onEraseBkgnd(HDC hdc)
    //{
    ///*
    //        RECT Rect;
    //        ::GetClientRect(m_hSelf, &Rect);
    //        ::ClientToScreen(m_hSelf, &Rect);
    //        ::ScreenToClient(m_hParent, &Rect);
    //        HDC hDC = ::GetDC(m_hParent);
    //        int m_Width = Rect.right - Rect.left;
    //        int m_Height = Rect.bottom - Rect.top;
    //        HDC hMemDC;
    //        hMemDC = ::CreateCompatibleDC(hDC);
    //        HBITMAP hOldbmp = (HBITMAP)::SelectObject(hMemDC, &m_hBmp);
    //        ::BitBlt(hMemDC, 0, 0,Rect.Width(),Rect.Height(),
    //            hDC,Rect.left, Rect.top, SRCCOPY);
    //        ::SelectObject(hMemDC, hOldbmp);
    //        ::ReleaseDC(m_hParent, hDC);
    //*/
    //
 //       HBRUSH hbrush = ::CreateSolidBrush(huys::orange);
    //    RECT rc;
    //    GetClientRect(m_hSelf, &rc);
    //    ::SetBkColor( hdc, huys::orange );
    //    ::FillRect( hdc , &rc, hbrush );
    //    return TRUE;
    //}

  //  BOOL onPaint()
  //  {
  //      //!! BeginPaint will eat previous control text drawing or other actions
  //      PAINTSTRUCT ps;
  //      HDC hdc;
  //      hdc = BeginPaint(m_hSelf, &ps);
  //      //hdc = ::GetDC(m_hSelf);
  //
  //      //onDraw(hdc);

  //      //::ReleaseDC(m_hSelf, hdc);
  //      EndPaint(m_hSelf, &ps);
  //      return FALSE;
  //  }
  //
  //  void onDraw(HDC hdc)
  //  {
        ////HBRUSH hbrush = ::CreateSolidBrush(huys::orange);
        ////RECT rc;
        ////GetClientRect(m_hSelf, &rc);
        ////::SetBkColor( hdc, huys::orange );
        ////::FillRect( hdc , &rc, hbrush );
        //SetBkMode(hdc, TRANSPARENT);
  //  }


    //BOOL onSize(WPARAM wParam, LPARAM lParam)
    //{
    //    hide();
    //    invalidate();
    //    update();
    //    show();
    //    return FALSE;
    //}
};

#endif // U_CONTACT_LISTBOX_H

