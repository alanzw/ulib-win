#ifndef U_LCD_H
#define U_LCD_H

// Make sure these messages are not used in your application already
// If in doubt, use RegisterWindowMessages

#define WM_OVERFLOW         (WM_USER + 1)
#define WM_UNDERFLOW        (WM_USER + 2)
#define WM_TCHANGE          (WM_USER + 3)

class ULCDCtrl : public UStatic
{
public:
    ULCDCtrl(HWND hParent, UINT nID, HINSTANCE hInst=GetModuleHandle(NULL))
        : UStatic(hParent, nID, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_NOTIFY;

        //COLORREF bkColor = ::GetSysColor( COLOR_3DFACE );
        m_hBackground = (HBRUSH)::CreateSolidBrush(huys::silver);
        m_hForeground = (HBRUSH)::CreateSolidBrush(huys::black);

        m_hNullPen = (HPEN)::CreatePen(PS_NULL, 0, huys::black);

        bFirstPaint = true;
        for (int i=0; i<7; ++i)
        {
            m_bSeg[i] = false;
        }
        low        = 0;
        high    = 9;
        gap        = 1;
        width    = 0;
        oldnum = num = 0;
    }

    ~ULCDCtrl()
    {
        ::DeleteObject(m_hBackground);
        ::DeleteObject(m_hForeground);
        ::DeleteObject(m_hNullPen);
    };

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
    }

    virtual BOOL onPaint()
    {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rc;

        this->getClientRect(&rc);

        if(bFirstPaint)
        {
            bFirstPaint = false;
            if(width == 0)
                width = rc.right / 4;

            ::PostMessage(m_hParent, WM_TCHANGE, m_nResourceID, 0);
        }


        hdc = BeginPaint(m_hSelf, &ps);

        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, m_hBackground);
        HPEN hOldPen = (HPEN)::SelectObject(hdc, m_hNullPen);
        ::Rectangle(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
        ::SelectObject(hdc, hOldBrush);
        //::SelectObject(hdc, hOldPen);

        // Delimit number
        if(num < low)
        {
            num = high;
            ::PostMessage(m_hParent, WM_UNDERFLOW , m_nResourceID , 0);
        }

        if(num > high)
        {
            num = low;
            ::PostMessage(m_hParent, WM_OVERFLOW, m_nResourceID , 0);
        }

        for (int i=0; i<7; ++i)
        {
            m_bSeg[i] = false;
        }

        // Coding of segments:
        //                  1
        //             ---------
        //        4    |    3    | 6
        //             ---------
        //        5    |         | 7
        //             ---------
        //               2

        // Set the ones that need to be set:
        switch(num)
        {
        case 0: m_bSeg[0] = m_bSeg[1] = m_bSeg[3] = m_bSeg[4] = m_bSeg[5] = m_bSeg[6] = true;    break;
        case 1:    m_bSeg[5]= m_bSeg[6] = true; break;
        case 2: m_bSeg[0] = m_bSeg[5] = m_bSeg[2] = m_bSeg[4] = m_bSeg[1] = true; break;
        case 3: m_bSeg[0] = m_bSeg[5] = m_bSeg[2] = m_bSeg[6] = m_bSeg[1] = true; break;
        case 4: m_bSeg[3]  = m_bSeg[2] = m_bSeg[5] = m_bSeg[6] = true;    break;
        case 5: m_bSeg[0]  = m_bSeg[3] = m_bSeg[2] = m_bSeg[6] = m_bSeg[1] = true; break;
        case 6: m_bSeg[0] = m_bSeg[3]  = m_bSeg[4] = m_bSeg[1] = m_bSeg[6] = m_bSeg[2] = true; break;
        case 7: m_bSeg[0]  = m_bSeg[5]  = m_bSeg[6] = true; break;
        case 8: m_bSeg[0] = m_bSeg[1] = m_bSeg[2] = m_bSeg[3] = m_bSeg[4] = m_bSeg[5] = m_bSeg[6] = true; break;
        case 9: m_bSeg[0] = m_bSeg[2] = m_bSeg[3] = m_bSeg[5] = m_bSeg[6] = m_bSeg[1] = true; break;
        case 10: m_bSeg[0] = m_bSeg[3] = m_bSeg[4] = m_bSeg[2] = m_bSeg[5] = m_bSeg[6] = true; break;  // A
        case 11: m_bSeg[3] = m_bSeg[4] = m_bSeg[1] = m_bSeg[2] = m_bSeg[6] = true; break;  // b
        case 12: m_bSeg[2] = m_bSeg[4] = m_bSeg[1] = true; break;  // c
        case 13: m_bSeg[2] = m_bSeg[4] = m_bSeg[1] = m_bSeg[6] = m_bSeg[5] = true; break;  // d
        case 14: m_bSeg[0] = m_bSeg[3] = m_bSeg[2] = m_bSeg[4] = m_bSeg[1] = true; break;  // e
        case 15: m_bSeg[0] = m_bSeg[3] = m_bSeg[2] = m_bSeg[4] = true; break;  // f
        }

        int vhalf = rc.bottom / 2;
        POINT poly[6];

        ///////////////////////////////////////////////////
        ///    0---------------------------1
        //        3--------------------2
        //

        poly[0].x = 0;                        poly[0].y = 0;
        poly[1].x = rc.right;                poly[1].y = 0;
        poly[2].x = rc.right - (width);        poly[2].y = width;
        poly[3].x = width;                    poly[3].y = width;

        if(m_bSeg[0])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 4);

        ///////////////////////////////////////////////////
        //        0--------------------1
        ///    3---------------------------2

        poly[0].x = (width );                poly[0].y = rc.bottom - (width );
        poly[1].x = rc.right - (width);        poly[1].y = rc.bottom - (width );
        poly[2].x = rc.right;                poly[2].y = rc.bottom ;
        poly[3].x = 0;                        poly[3].y = rc.bottom ;

        if(m_bSeg[1])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 4);

        ///////////////////////////////////////////////////
        //        0--------------------1
        ///    5---------------------------2
        ///        4--------------------3
        //
        poly[0].x = width ;                    poly[0].y = vhalf - width/2 + gap;
        poly[1].x = rc.right - width;        poly[1].y = vhalf - width/2 + gap;
        poly[2].x = rc.right;                poly[2].y = vhalf;
        poly[3].x = rc.right - width;        poly[3].y = vhalf + width/2 + gap;
        poly[4].x = width;                    poly[4].y = vhalf + width/2 + gap;
        poly[5].x = 0;                        poly[5].y = vhalf + gap;

        if(m_bSeg[2])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 6);

        //////////////////////////////////////////////////
        //     0
        //  |  3
        //  | |
        //  |  2
        //   1

        poly[0].x = 0;                        poly[0].y = gap;
        poly[1].x = 0;                        poly[1].y = vhalf;
        poly[2].x = width;                    poly[2].y = vhalf - (width/2);
        poly[3].x = width;                    poly[3].y = width + gap;

        if(m_bSeg[3])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 4);

        poly[0].y += vhalf + gap;
        poly[1].y += vhalf -  gap;
        poly[2].y += vhalf -  gap - width/2;
        poly[3].y += vhalf - width/2 + gap;

        if(m_bSeg[4])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 4);

        //////////////////////////////////////////////////
        //            3
        //      0  |
        //       | |
        //      1  |
        //          2

        poly[0].x = rc.right - (width);            poly[0].y = (width)  + gap;
        poly[1].x = rc.right - (width);            poly[1].y = vhalf - (width/2);
        poly[2].x = rc.right;                    poly[2].y = vhalf - gap ;
        poly[3].x = rc.right;                    poly[3].y = gap;

        if(m_bSeg[5])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 4);

        poly[0].y += vhalf - width/2 + gap;
        poly[1].y += vhalf - width/2 - gap;
        poly[2].y += vhalf;
        poly[3].y += vhalf;

        if(m_bSeg[6])
            ::SelectObject(hdc, m_hForeground);
        else
            ::SelectObject(hdc, m_hBackground);

        ::Polygon(hdc, poly, 4);

        ::SelectObject(hdc, hOldBrush);
        ::SelectObject(hdc, hOldPen);
        ::SelectObject(hdc, hOldPen);
        if(oldnum != num)
        {
            ::PostMessage(m_hParent, WM_TCHANGE, m_nResourceID, 0);
        }
        oldnum = num;


        EndPaint(m_hSelf, &ps);

        return FALSE;
    }

    virtual BOOL onLButtonDown(WPARAM,LPARAM)
    {
        //::MessageBox(NULL, "LBUTTONDOWN", "LCD", MB_OK);
        ::SetCapture(m_hSelf);
        //if (num <10)
        //{

        //}

        return FALSE;
    }

    virtual BOOL onLButtonUp(WPARAM,LPARAM)
    {
        //::MessageBox(NULL, "LBUTTONDUP", "LCD", MB_OK);
        ::ReleaseCapture();
        return TRUE;
    }

    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        //::MessageBox(m_hSelf, "MOUSEMOVE", "LCD", MB_OK);
        if (wParam & MK_LBUTTON)
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            if (x-oldpoint.x > 10)
            {
                num++;
                ::InvalidateRect(m_hSelf, NULL, TRUE);

                oldpoint.x = x;
            }

            if (oldpoint.x - x > 10)
            {
                num--;
                ::InvalidateRect(m_hSelf, NULL, TRUE);

                oldpoint.x = x;
            }

            UNREFERENCED_LOCAL_VARIABLE(y);
        }


        return TRUE;
    }
private:
    int gap;
    int width;
    int  low, high;
    int  num, oldnum;

    POINT oldpoint;
    //
    bool bFirstPaint;
    bool m_bSeg[6];

    //
    HBRUSH m_hBackground;
    HBRUSH m_hForeground;
    HPEN m_hNullPen;
};

#endif // U_LCD_H
