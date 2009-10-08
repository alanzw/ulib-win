#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
    LOGFONT logfont;
    GLuint nFontList;
    GLYPHMETRICSFLOAT agmf[256];
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {}

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        glViewport(0, 0, rc.right-rc.left, rc.bottom-rc.top);
        //UGlut::ResizeScene(rc.right-rc.left, rc.bottom-rc.top);

        //logfont.lfHeight = -12;        // setup font characteristics
        //logfont.lfWidth = 0;
        //logfont.lfEscapement = 0;
        //logfont.lfOrientation = 0;
        //logfont.lfWeight = FW_NORMAL;
        //logfont.lfItalic = FALSE;
        //logfont.lfUnderline = TRUE;
        //logfont.lfStrikeOut = FALSE;
        //logfont.lfCharSet = ANSI_CHARSET;
        //logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
        //logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        //logfont.lfQuality = PROOF_QUALITY;
        //logfont.lfPitchAndFamily = DEFAULT_PITCH || FF_ROMAN;
        //strcpy(logfont.lfFaceName,"Arial");

        ////HDC hdc = this->getDC();

        //HFONT hFont = CreateFontIndirect(&logfont);
        //SelectObject (m_hdc, hFont);
        ////nFontList = glGenLists(128);
        //wglUseFontOutlines(m_hdc, 0, 128, 1000, 0.0f, 0.5f,
        //    WGL_FONT_POLYGONS, agmf);
        //DeleteObject(hFont);
        // make the color a deep blue hue
        //glClearColor(0.0F, 0.0F, 0.4F, 1.0F);

        // make the shading smooth
        glShadeModel(GL_SMOOTH);

        // clear the color buffers
        glClear(GL_COLOR_BUFFER_BIT);

        return TRUE;
    }

    virtual BOOL animate()
    {
        /* rotate a triangle around */
        glClear(GL_COLOR_BUFFER_BIT);
        //glLoadIdentity();
        //glTranslatef(0.0f, 0.0f, -5.0f);
        //glScalef(2.0f, 2.0f, 2.0f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(0,  1);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2i(-1, -1);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(1, -1);
        glEnd();

        // specify a red triangle
        //glBegin(GL_TRIANGLES);
        //glColor3f(1.0F, 0.0F, 0.0F);
        //glVertex2f(10.0F, 10.0F);
        //glVertex2f(250.0F, 50.0F);
        //glVertex2f(105.0F, 280.0F);
        //glEnd();

        // create bitmaps for the device context font's first 256 glyphs
        wglUseFontBitmaps(m_hdc, 0, 256, 1000);

        // move bottom left, southwest of the red triangle
        glRasterPos2f(0.6F, 0.8F);

        // set up for a string-drawing display list call
        glListBase(1000);

        // draw a string using font display lists
        glCallLists(12, GL_UNSIGNED_BYTE, "Red Triangle");

        // get all those commands to execute
        glFlush();

        // delete our 256 glyph display lists
        glDeleteLists(1000, 256);

        glListBase(1000);
        glCallLists (11, GL_UNSIGNED_BYTE, "Text Output");

        glFlush();
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UGLDialog::onCancel();
        default:
            return UGLDialog::onChar(wParam, lParam);
        }
    }
};

UDLGAPP_T(MyGLDialog, IDD_TEST);

