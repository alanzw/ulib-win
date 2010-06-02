#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#include "ugdi.h"
#include "colors.h"

#include "ulistbox.h"
#include "ustatic.h"

#include "adt/uautoptr.h"

#include "aui/aui_label.h"
#include "aui/aui_tracewin.h"
#include "aui/aui_glctrl.h"
#include "aui/aui_dockwin.h"

#include "u3dcreator.h"

#ifdef _MSC_VER

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#endif // _MSC_VER


void getColor(float v, GLfloat vdColor[3])
{
    GLfloat v_min = 0.f;
    GLfloat v_max = 4.f;

    GLfloat r1 = 0.0f;
    GLfloat r2 = 128.0f;
    GLfloat g1 = 0.0f;
    GLfloat g2 = 255.0f;
    GLfloat b1 = 0.0f;
    GLfloat b2 = 0.0f;

    vdColor[0] = (GLfloat)((r1+(r2-r1)*(v-v_min)/(v_max-v_min))/255.0);
    vdColor[1] = (GLfloat)((g1+(g2-g1)*(v-v_min)/(v_max-v_min))/255.0);
    vdColor[2] = (GLfloat)((b1+(b2-b1)*(v-v_min)/(v_max-v_min))/255.0);
}

/*

    (i, j)   ------- (i, j+1)
       |                |
       |                |
       |                |
    (i+1, j) ------- (i+1, j+1)
 */

void my_render()
{
    GLfloat vdColor[3];
    GLfloat top = .5f;
    GLfloat bottom = -0.5f;
    GLfloat left = -.5f;
    GLfloat right = .5f;
    GLfloat m = 2;
    GLfloat n = 1;
    GLfloat dy = (bottom - top) / m;
    GLfloat dx = (right - left) / n;

    GLfloat w[][3] = { 1.0f, 2.0f, 3.f, 4.f, 1.0f, 2.0f, 3.f, 4.f, 2.f };

    GLfloat value;

    int i, j;

    for (i=0; i<m; ++i)
    {
        top = (GLfloat)(bottom - (i+1)*dy);
        bottom = (GLfloat)(bottom - i*dy);
        for (j=0; j<n; ++j)
        {
            left = (GLfloat)(left + j*dx);
            right = (GLfloat)(left + (j+1)*dx);
            glBegin(GL_POLYGON);
            value = w[i][j];
            getColor(value, vdColor);
            glColor3fv(vdColor);
            glVertex2f(left, bottom);
            value = w[i][j+1];
            getColor(value, vdColor);
            glColor3fv(vdColor);
            glVertex2f(right, bottom);
            value = w[i+1][j+1];
            getColor(value, vdColor);
            glColor3fv(vdColor);
            glVertex2f(right, top);
            value = w[i+1][j];
            getColor(value, vdColor);
            glColor3fv(vdColor);
            glVertex2f(left, top);
            glEnd();
        }
    }

}

class UMyWindow : public UBaseWindow
{
    enum {
        ID_TERMINAL = 11002
    };
public:
    UMyWindow()
        : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("U3DCreator 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        win = new AUI::UTraceWindow(this);
        //win->setPos(100, 100, 200, 200);
        win->create();

        glctl = new AUI::UGLCtrl(this);
        glctl->setPos(50, 50, 600, 600);
        glctl->setInterval(10);
        glctl->addRender(&my_render);
        glctl->create();

        dockWin = new AUI::UDockWindow(this);
        dockWin->create();

        return UBaseWindow::onCreate();
    }

    //
    /* virtual */ void onDraw(HDC hdc)
    {

    }
    //
    /* virtual */ BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::black);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
        return TRUE;
    }

    //
    /* virtual */ BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            return onMenuAbout();
        case IDM_NUMBER:
            return onMenuNumber();
        case IDM_EXIT:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    /* virtual */ BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        case VK_SPACE:
            return onKeySpace();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        //::SendMessage(win, WM_SIZE,0,0);
        ::SendMessage(dockWin, WM_SIZE,0,0);
        return UBaseWindow::onSize(wParam, lParam);
    }
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("U3DCreator v0.0.1"), _T("About"));
        return FALSE;
    }

    BOOL onMenuNumber()
    {
        if (!win->isVisible())
        {
            win->show();
        }

        win->addLine("You Clicked Me!");
        return FALSE;
    }

    BOOL onKeySpace()
    {
        static bool bFlag = false;

        bFlag = (bFlag == true ? false : true);

        if (bFlag)
        {
            glctl->resetRenders();
        }
        else
        {
            glctl->addRender(&my_render);
        }

        return FALSE;
    }

private:
    AUI::UTraceWindowP win;
    AUI::UGLCtrlP glctl;
    AUI::UDockWindowP dockWin;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

