#define WINVER 0x0501
#define _WIN32_WINNT  0x0501

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "ubutton.h"
#include "colors.h"

UButton::UButton(HWND hParent, UINT nResource, HINSTANCE hInst)
:UControl(hParent, nResource, hInst)
{
    m_dwStyles |= BS_DEFPUSHBUTTON;
}

UButton::UButton(UBaseWindow * pWndParent,UINT nID)
: UControl(pWndParent, nID)
{
    m_dwStyles |= BS_DEFPUSHBUTTON;
}


UButton::~UButton()
{}

BOOL UButton::create()
{
    return UControl::create(_T("BUTTON"), _T(""));
}

BOOL UButton::click()
{
    return this->sendMsg(BM_CLICK);
}

#if ( (_MSC_VER <= 1200) || !defined(_MSC_VER) )
#ifndef BCM_FIRST
  #define BCM_FIRST               0x1600      // Button control messages
#endif // BCM_FIRST
  #define BCM_GETIDEALSIZE        (BCM_FIRST + 0x0001)
#endif

BOOL UButton::getIdealSize(SIZE *pSize)
{
    return this->sendMsg(BCM_GETIDEALSIZE, 0, (LPARAM)pSize);
}

#if WINVER >= 0x0600
USplitButton::USplitButton(HWND hParent, UINT nResource, HINSTANCE hInst)
: UButton(hParent, nResource, hInst)
{}

USplitButton::~USplitButton()
{}
#endif

UCheckButton::UCheckButton(HWND hParent, UINT nResource, HINSTANCE hInst)
: UButton(hParent, nResource, hInst)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_AUTOCHECKBOX;
}

UCheckButton::~UCheckButton()
{}

BOOL UCheckButton::check()
{
    return this->sendMsg(BM_SETCHECK, (WPARAM)BST_CHECKED);
}

BOOL UCheckButton::uncheck()
{
    return this->sendMsg(BM_SETCHECK, (WPARAM)BST_UNCHECKED);
}

URadioButton::URadioButton(HWND hParent, UINT nResource, HINSTANCE hInst)
: UButton(hParent, nResource, hInst)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_AUTORADIOBUTTON;
}

URadioButton::URadioButton(UBaseWindow * pWndParent,UINT nID)
: UButton(pWndParent, nID)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_AUTORADIOBUTTON;
}


URadioButton::~URadioButton()
{}

BOOL URadioButton::check()
{
    return this->sendMsg(BM_SETCHECK, (WPARAM)BST_CHECKED);
}

BOOL URadioButton::uncheck()
{
    return this->sendMsg(BM_SETCHECK, (WPARAM)BST_UNCHECKED);
}


BOOL URadioButton::isChecked()
{
    return (BST_CHECKED == this->sendMsg(BM_GETCHECK));
}

/*
 *  GroupBox
 */
UGroupBox::UGroupBox(HWND hParent, UINT nResource, HINSTANCE hInst)
: UButton(hParent, nResource, hInst)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_GROUPBOX;
}

UGroupBox::~UGroupBox()
{}

UGroupBox::UGroupBox(UBaseWindow *pWndParent, UINT nID)
: UButton(pWndParent, nID)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_GROUPBOX;
}

UIconButton::UIconButton(HWND hParent, UINT nResource, HINSTANCE hInst)
: UButton(hParent, nResource, hInst)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_PUSHBUTTON|BS_ICON;
}

UIconButton::~UIconButton()
{};

#if ( (_MSC_VER <= 1200) || !defined(_MSC_VER) )
#if WINVER >= 0x0501
#define BCM_FIRST               0x1600      // Button control messages
#define BUTTON_IMAGELIST_ALIGN_LEFT     0
#define BUTTON_IMAGELIST_ALIGN_RIGHT    1
#define BUTTON_IMAGELIST_ALIGN_TOP      2
#define BUTTON_IMAGELIST_ALIGN_BOTTOM   3
#define BUTTON_IMAGELIST_ALIGN_CENTER   4       // Doesn't draw text

typedef struct
{
    HIMAGELIST  himl;   // Index: Normal, hot pushed, disabled. If count is less than 4, we use index 1
    RECT        margin; // Margin around icon.
    UINT        uAlign;
} BUTTON_IMAGELIST, *PBUTTON_IMAGELIST;

#define BCM_SETIMAGELIST        (BCM_FIRST + 0x0002)
#define Button_SetImageList(hwnd, pbuttonImagelist)\
    (BOOL)SNDMSG((hwnd), BCM_SETIMAGELIST, 0, (LPARAM)(pbuttonImagelist))

#define BCM_GETIMAGELIST        (BCM_FIRST + 0x0003)
#define Button_GetImageList(hwnd, pbuttonImagelist)\
    (BOOL)SNDMSG((hwnd), BCM_GETIMAGELIST, 0, (LPARAM)(pbuttonImagelist))

#define BCM_SETTEXTMARGIN       (BCM_FIRST + 0x0004)
#define Button_SetTextMargin(hwnd, pmargin)\
    (BOOL)SNDMSG((hwnd), BCM_SETTEXTMARGIN, 0, (LPARAM)(pmargin))
#define BCM_GETTEXTMARGIN       (BCM_FIRST + 0x0005)
#define Button_GetTextMargin(hwnd, pmargin)\
    (BOOL)SNDMSG((hwnd), BCM_GETTEXTMARGIN, 0, (LPARAM)(pmargin))

typedef struct tagNMBCHOTITEM
{
    NMHDR   hdr;
    DWORD   dwFlags;           // HICF_*
} NMBCHOTITEM, * LPNMBCHOTITEM;

#define BCN_HOTITEMCHANGE       (BCN_FIRST + 0x0001)

#define BST_HOT            0x0200
#endif // WINVER >= 0x0501
#endif

BOOL UIconButton::setIcon(HICON hicon)
{

    //::SendMessage(m_hSelf, BM_SETSTYLE,  (WPARAM) BS_ICON, (LPARAM)TRUE);
/*
    HANDLE hImage = (HANDLE)::SendMessage(m_hSelf, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hicon);

    if (!hImage)
    {
        return FALSE;
    }
*/

#if WINVER >= 0x0501
    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    HIMAGELIST himlIcons;  // handle to new image list
    himlIcons = ImageList_Create( 32, 32, ILC_MASK|ILC_COLORDDB|ILC_COLOR32, 1, 0 );
    ImageList_AddIcon(himlIcons, hicon);
    BUTTON_IMAGELIST bil = {0};
    bil.himl = himlIcons;
    bil.margin.left = 5;
    bil.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT;

    ::SendMessage(m_hSelf, BCM_SETIMAGELIST, 0, (LPARAM)(PBUTTON_IMAGELIST)&bil);
#endif // WINVER >= 0x0501


    return TRUE;
}

UBitmapButton::UBitmapButton(HWND hParent, UINT nResource, HINSTANCE hInst)
: UButton(hParent, nResource, hInst)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_BITMAP;
}

UBitmapButton::~UBitmapButton()
{};


BOOL UBitmapButton::setBitmap( HBITMAP hbm )
{
    HANDLE hImage = (HANDLE)::SendMessage(m_hSelf, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hbm);

    if (hImage)
    {
        return TRUE;
    }

    return FALSE;
}

UOwnerDrawnButton::UOwnerDrawnButton(HWND hParent, UINT nResource, HINSTANCE hInst)
:  UButton(hParent, nResource, hInst)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_OWNERDRAW;
}

UOwnerDrawnButton::UOwnerDrawnButton(UBaseWindow *pWndParent, UINT nID)
: UButton(pWndParent, nID)
{
    m_dwStyles &= ~BS_DEFPUSHBUTTON;
    m_dwStyles |= BS_OWNERDRAW;
}

UOwnerDrawnButton::~UOwnerDrawnButton()
{}

BOOL UOwnerDrawnButton::create()
{
    BOOL bRet = UButton::create();
    bRet &= this->subclassProc();
    return bRet;
}

BOOL UOwnerDrawnButton::onDrawItem(WPARAM wParam, LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDrawItem;
    HDC hdc;
    HBRUSH hbrush = ::CreateSolidBrush(huys::orange);

    lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
    //printfMsg("info", "CtrlType: %d", lpDrawItem->CtlType);
    hdc = lpDrawItem->hDC;
    ::SetTextColor( hdc , ::GetSysColor( COLOR_HIGHLIGHTTEXT ) );
    ::SetBkColor( hdc, huys::orange );
    ::FillRect( hdc , &lpDrawItem->rcItem, hbrush );

    TCHAR buf[1024];
    this->getWindowText(buf, 1023);
    ::TextOut( hdc,
               lpDrawItem->rcItem.left+5,
               lpDrawItem->rcItem.top+5,
               buf,
               _tcslen(buf)
             );
    return TRUE;
}


UIconTextButton::UIconTextButton( HWND hParent, UINT nResource, HINSTANCE hInst )
: UIconButton(hParent, nResource, hInst)
{
    m_dwStyles |= BS_RIGHT;
}

UIconTextButton::~UIconTextButton()
{}

