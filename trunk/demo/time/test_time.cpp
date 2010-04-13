#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uicon.h"
#include "ubutton.h"
#include "utime.h"
#include "ustatic.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_TIMER_UPDATE = 1021
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        UTime time;
        time.getSysTime();

        huys::ADT::UStringAnsi text;
        text.format("SysTime: %d-%d-%d %d:%d:%d",
                    time.year(), time.month(), time.day(),
                    time.hour(), time.minute(), time.second());

        m_label = new UStatic(m_hDlg, -1, m_hInst);
        m_label->setPos(100, 100, 200, 100);
        m_label->setText(text);
        m_label->create();

        setTimer(ID_TIMER_UPDATE, 1000);

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case 0:
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    BOOL onClose()
    {
        killTimer(ID_TIMER_UPDATE);
        return UDialogBox::onClose();
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if (ID_TIMER_UPDATE == wParam)
        {
            UTime time;
            time.getSysTime();

            huys::ADT::UStringAnsi text;
            text.format("SysTime: %d-%d-%d %d:%d:%d",
                    time.year(), time.month(), time.day(),
                    time.hour(), time.minute(), time.second());
            m_label->setWindowText(text);
        }
        return FALSE;
    }

private:
    huys::ADT::UAutoPtr<UStatic> m_label;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

