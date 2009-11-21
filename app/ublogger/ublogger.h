#include "resource.h"

#include "uwinapp.h"
#include "ubasewindow.h"

#include "ubutton.h"
#include "uedit.h"

#include "adt/uvector.h"

class UBloggerWindow : public UBaseWindow
{
    enum {
        IDC_BN_CONVERT  = 122,
        IDC_ED_ORIGIN   = 200,
        IDC_ED_CONVERT  = 201
    };
public:
    UBloggerWindow(HINSTANCE hInst)
    : UBaseWindow(NULL, hInst)
    {
        this->setTitle(_T("Blog Code Translator 0.01"));
    }

    ~UBloggerWindow()
    {
        CHECK_PTR(m_pEdtOrigin);
        CHECK_PTR(m_pEdtConvert);
        CHECK_PTR(m_pBtnConvert);
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        m_pEdtOrigin = new UEdit(*this, IDC_ED_ORIGIN, getInstance());
        m_pEdtOrigin->setStyles(WS_BORDER | ES_MULTILINE);
        m_pEdtOrigin->setPos(50, 50, 300, 400);
        m_pEdtOrigin->create();

        m_pEdtConvert = new UEdit(*this, IDC_ED_CONVERT, getInstance());
        m_pEdtConvert->setStyles(WS_BORDER | ES_MULTILINE);
        m_pEdtConvert->setPos(500, 50, 300, 400);
        m_pEdtConvert->create();

        m_pBtnConvert = new UButton(*this, IDC_BN_CONVERT, getInstance());
        m_pBtnConvert->setPos(370, 220, 100, 100);
        m_pBtnConvert->create();
        m_pBtnConvert->setWindowText(_T(">>>"));

        return UBaseWindow::onCreate();
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_CONVERT:
            return this->onBnConvert();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    UEdit *m_pEdtOrigin;
    UEdit *m_pEdtConvert;
    UButton *m_pBtnConvert;
private:
    BOOL onBnConvert()
    {
        return FALSE;
    }
};

class UBloggerApp : public UWinApp
{
public:
    bool init( HINSTANCE hInst /*= NULL*/ )
    {
        m_pMainWindow = new UBloggerWindow(hInst);
        m_pMainWindow->create();

        return true;
    }
};

