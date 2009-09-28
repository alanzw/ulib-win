#ifndef U_CURSOR_H
#define U_CURSOR_H

#include "uobject.h"

class ULIB_API UCursor : public UUserObject
{
public:
    UCursor();
    ~UCursor();
    //
    bool destroy();
    //
    bool getCursorInfo();
    //
    bool getCursorPos(LPPOINT lpPoint);
    //
    HCURSOR getCursor();
    //
    /*
     * IDC_APPSTARTING
     *     Standard arrow and small hourglass
     * IDC_ARROW
     *     Standard arrow
     * IDC_CROSS
     *     Crosshair
     * IDC_HAND
     *     Windows 98/Me, Windows 2000/XP: Hand
     * IDC_HELP
     *     Arrow and question mark
     * IDC_IBEAM
     *     I-beam
     * IDC_ICON
     *     Obsolete for applications marked version 4.0 or later.
     * IDC_NO
     *     Slashed circle
     * IDC_SIZE
     *     Obsolete for applications marked version 4.0 or later. Use IDC_SIZEALL.
     * IDC_SIZEALL
     *     Four-pointed arrow pointing north, south, east, and west
     * IDC_SIZENESW
     *     Double-pointed arrow pointing northeast and southwest
     * IDC_SIZENS
     *     Double-pointed arrow pointing north and south
     * IDC_SIZENWSE
     *     Double-pointed arrow pointing northwest and southeast
     * IDC_SIZEWE
     *     Double-pointed arrow pointing west and east
     * IDC_UPARROW
     *     Vertical arrow
     * IDC_WAIT
     *     Hourglass
     */
    bool loadStdCursor(LPCTSTR lpCursorName);
    //
    bool loadFromRes(HINSTANCE hInst, LPCTSTR lpCursorName);
    bool loadFromResID(HINSTANCE hInst, UINT uID);
    //
    bool loadFromFile(LPCTSTR lpFileName);

    //
    bool createCursor( HINSTANCE hInst,
                       int xHotSpot,
                       int yHotSpot,
                       int nWidth,
                       int nHeight,
                       const VOID *pvANDPlane,
                       const VOID *pvXORPlane );

    //
    bool use();
    bool restore();
    //
    bool getClipCursor(LPRECT lpRect);
    bool clipCursor(const RECT *lpRect);

    //
    bool copy(HCURSOR hCursor);

    //
    bool setPos(int x, int y);
private:
    CURSORINFO m_cursorInfo;
    HCURSOR m_hOldCursor;
};

#endif // U_CURSOR_H

