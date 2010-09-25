#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglut.h"
#include "uicon.h"
#include "umsg.h"

#if defined(_BASETSD_H)
 #define _BASETSD_H_
#endif // defined(_BASETSD_H_)

#if defined(__GNUC__) && !defined(HAVE_BOOLEAN)
 #define HAVE_BOOLEAN
#endif

#include "jpeglib.h"

#define TGA_RGB        2
#define TGA_A        3
#define TGA_RLE        10

// This stores the important jpeg data
struct tImageJPG
{
    int rowSpan;
    int sizeX;
    int sizeY;
    unsigned char *data;
};

tImageJPG *Load_JPEG(const char *strfilename);
void Decompress_JPEG(jpeg_decompress_struct* cInfo, tImageJPG *pImgData);
void JPEG_Texture(UINT textureArray[], LPSTR strFileName, int ID);

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        JPEG TEXTURE LOADER
/////////////////////////////////////////////////////////////////////////////////////////////////
void JPEG_Texture(UINT textureArray[], LPSTR strFileName, int ID)
{
    if(!strFileName)    return;

    tImageJPG *pBitMap = Load_JPEG(strFileName);

    if(pBitMap == NULL)    exit(0);

    glGenTextures(1, &textureArray[ID]);
    glBindTexture(GL_TEXTURE_2D, textureArray[ID]);

    gluBuild2DMipmaps( GL_TEXTURE_2D,
                       3,
                       pBitMap->sizeX,
                       pBitMap->sizeY,
                       GL_RGB,
                       GL_UNSIGNED_BYTE,
                       pBitMap->data );

    glTexParameteri( GL_TEXTURE_2D,
                     GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );

    glTexParameteri( GL_TEXTURE_2D,
                     GL_TEXTURE_MAG_FILTER,
                     GL_LINEAR_MIPMAP_LINEAR );

    if (pBitMap)
    {
        if (pBitMap->data)
        {
            free(pBitMap->data);
        }
        free(pBitMap);
    }
}

tImageJPG *Load_JPEG(const char *strfilename)
{
    struct jpeg_decompress_struct cInfo;
    tImageJPG *pImgData = NULL;
    FILE *pFile;

    if((pFile = fopen(strfilename, "rb")) == NULL)
    {
        MessageBox(NULL, "Error loading jpg file.", "ERROR", MB_OK);
        return NULL;
    }

    jpeg_error_mgr jerr;

    cInfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cInfo);

    jpeg_stdio_src(&cInfo, pFile);

    pImgData = (tImageJPG*)malloc(sizeof(tImageJPG));

    Decompress_JPEG(&cInfo, pImgData);

    jpeg_destroy_decompress(&cInfo);

    fclose(pFile);

    return pImgData;
}


void Decompress_JPEG(jpeg_decompress_struct* cInfo, tImageJPG *pImgData)
{
    jpeg_read_header(cInfo, TRUE);

    jpeg_start_decompress(cInfo);

    pImgData->rowSpan = cInfo->image_width * cInfo->num_components;
    pImgData->sizeX   = cInfo->image_width;
    pImgData->sizeY   = cInfo->image_height;

    pImgData->data = new unsigned char[pImgData->rowSpan * pImgData->sizeY];

    unsigned char** rowPtr = new unsigned char*[pImgData->sizeY];
    for (int i = 0; i < pImgData->sizeY; i++)
        rowPtr[i] = &(pImgData->data[i*pImgData->rowSpan]);

    int rowsRead = cInfo->output_height-1;
    while (cInfo->output_scanline < cInfo->output_height)
    {
        rowsRead -= jpeg_read_scanlines(cInfo, &rowPtr[rowsRead], cInfo->output_height - rowsRead);
    }
    delete [] rowPtr;

    jpeg_finish_decompress(cInfo);
}



class UGLWindow : public UBaseWindow
{
private:
    enum {
        MA_PAN = 1,    /* pan state bit     */
        MA_ROTATE,     /* rotate state bits */
        MA_ZOOM        /* zoom state bit    */
    };

    GLfloat trans[3];    /* current translation */
    GLfloat rot[2];      /* current rotation */

    int omx, omy, mx, my;
    GLuint state;

    GLboolean left;    /* left button currently down? */
    GLboolean right;   /* right button currently down? */

    //
    float rotqube;
public:
    UGLWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst),
      m_hdc(0), m_hrc(0), m_bFullscreen(FALSE)
    {
        setTitle(_T("OpenGL Window"));

        trans[0] = 0.0f;
        trans[1] = 0.0f;
        trans[2] = -7.0f;
        rot[0] = rot[1] = 0.0f;
        state   = 0;
        left  = GL_FALSE;
        right = GL_FALSE;

        //
        rotqube = 0.0f;
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setStyles(CS_OWNDC);
        return FALSE;
    }

    BOOL onPreCreateWindow()
    {
        if (IDYES == showYesNoMsgbox(_T("Fullscreen?"), _T("Start Fullscreen?")))
        {
            m_bFullscreen = TRUE;
        }

        int width= 640;
        int height = 480;
        int bits=16;

        setPos(0, 0, width, height);

        if (m_bFullscreen)
        {
            DEVMODE dmScreenSettings;                                // Device Mode
            memset(&dmScreenSettings,0,sizeof(dmScreenSettings));    // Makes Sure Memory's Cleared
            dmScreenSettings.dmSize=sizeof(dmScreenSettings);        // Size Of The Devmode Structure
            dmScreenSettings.dmPelsWidth    = width;                // Selected Screen Width
            dmScreenSettings.dmPelsHeight    = height;                // Selected Screen Height
            dmScreenSettings.dmBitsPerPel    = bits;                    // Selected Bits Per Pixel
            dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

            // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
            if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            {
                // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
                if (showYesNoMsgbox(_T("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card.")
                    _T("Use Windowed Mode Instead?"),_T("OpenGL"))==IDYES)
                {
                    m_bFullscreen=FALSE;        // Windowed Mode Selected.  Fullscreen = FALSE
                }
                else
                {
                    // Pop Up A Message Box Letting User Know The Program Is Closing.
                    showMsg(_T("Program Will Now Close."),_T("ERROR"));
                    return FALSE;                                    // Return FALSE
                }
            }

        }

        if (m_bFullscreen)
        {
            setStyles(WS_POPUP);
            setExStyles(WS_EX_APPWINDOW);
            ::ShowCursor(FALSE);
        }
        else
        {
            //setStyles(WS_OVERLAPPEDWINDOW);
            setExStyles(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
        }

        adjustWindowRectEx();

        return TRUE;
    }

    BOOL onCreate()
    {
        initGL();

        if (!m_bFullscreen)
        {
            m_uico.loadIconEx(::GetModuleHandle(NULL), IDI_APP);
            this->setIconBig(m_uico);
        }
        else
        {
            //ReSizeGLScene(640, 480); // Set Up Our Perspective GL Screen
        }
        setTopMost();

        return UBaseWindow::onCreate();
    }

    BOOL onDestroy()
    {
        UGlut::DisableOpenGL(getHandle(), m_hdc, m_hrc);

        return UBaseWindow::onDestroy();
    }

    void onDraw(HDC hdc)
    {
        render();
        SwapBuffers(m_hdc);
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

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        return FALSE;
    }

    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        if (state) {
            omx = mx;
            omy = my;
            mx = LOWORD(lParam);
            my = HIWORD(lParam);
            /* Win32 is pretty braindead about the x, y position that
                it returns when the mouse is off the left or top edge
                of the window (due to them being unsigned). therefore,
                roll the Win32's 0..2^16 pointer co-ord range to the
                more amenable (and useful) 0..+/-2^15. */
            if(mx & 1 << 15) mx -= (1 << 16);
            if(my & 1 << 15) my -= (1 << 16);
            update(state, omx, mx, omy, my);
            //PostMessage(hWnd, WM_PAINT, 0, 0);
        }
        return UBaseWindow::onMouseMove(wParam, lParam);
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(getHandle());
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        state |= MA_PAN;
        return UBaseWindow::onLButtonDown(wParam, lParam);
    }

    virtual BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(getHandle());
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        state |= MA_ROTATE;
        return UBaseWindow::onRButtonDown(wParam, lParam);
    }

    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        state = 0;
        return UBaseWindow::onLButtonUp(wParam, lParam);
    }

    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        state = 0;
        return UBaseWindow::onRButtonUp(wParam, lParam);
    }

    virtual BOOL onMButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::SetCapture(getHandle());
        state |= MA_ZOOM;
        return UBaseWindow::onMButtonDown(wParam, lParam);
    }

    virtual BOOL onMButtonUp(WPARAM wParam, LPARAM lParam)
    {
        ::ReleaseCapture();
        state = 0;
        return UBaseWindow::onMButtonUp(wParam, lParam);
    }
private:
    HDC m_hdc;
    HGLRC m_hrc;
    UIcon m_uico;

    BOOL m_bFullscreen;

    UINT TextureArray[1];
public:
    BOOL initGL()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);

        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
        glClearDepth(1.0f);                                    // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

        glEnable(GL_TEXTURE_2D);

        JPEG_Texture(TextureArray, "./text.jpg", 0);

        return TRUE;
    }


    void ReSizeGLScene(GLsizei width, GLsizei height)        // Resize And Initialize The GL Window
    {
        if (height==0)                                        // Prevent A Divide By Zero By
        {
            height=1;                                        // Making Height Equal One
        }

        glViewport(0,0,width,height);                        // Reset The Current Viewport

        glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
        glLoadIdentity();                                    // Reset The Projection Matrix

        // Calculate The Aspect Ratio Of The Window
        gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

        glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
        glLoadIdentity();                                    // Reset The Modelview Matrix
    }

    void render()
    {
        //glViewport(0, 0, 640.0, 480.0);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glPushMatrix();

        glTranslatef(trans[0], trans[1], trans[2]);
        glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
        glRotatef(rot[1], 0.0f, 1.0f, 0.0f);

        glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

        glBegin(GL_QUADS);
            // top left vertex
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1, 1, 0);
            // bottom left vertex
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1, -1, 0);
            // bottom right vertex
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(1, -1, 0);
            // top right vertex
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(1, 1, 0);
        glEnd();

        glPopMatrix();
        glFlush();

        // !!!!!
        SwapBuffers(m_hdc);
    }
 private:
    void update(int state, int ox, int nx, int oy, int ny)
    {
        int dx = ox - nx;
        int dy = ny - oy;

        switch(state) {
        case MA_PAN:
            trans[0] -= dx / 100.0f;
            trans[1] -= dy / 100.0f;
            break;
        case MA_ROTATE:
            rot[0] += (dy * 180.0f) / 500.0f;
            rot[1] -= (dx * 180.0f) / 500.0f;

#define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
            clamp(rot[0]);
            clamp(rot[1]);
            break;
        case MA_ZOOM:
            trans[2] -= (dx+dy) / 100.0f;
            break;
        }
    }
};

class UGLWinApp : public UWinApp
{
    BOOL m_bQuit;
public:
    UGLWinApp()
    : m_bQuit(FALSE)
    {}

    BOOL run()
    {
        m_pMainWindow->show();
        m_pMainWindow->update();

        //BOOL bQuit = FALSE;
        MSG msg;

        while (!m_bQuit)
        {
            // check for messages
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                // handle or dispatch messages
                if ( msg.message == WM_QUIT)
                {
                    m_bQuit = TRUE;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else
            {
                // OpenGL animation code goes here
                static_cast<UGLWindow *>(m_pMainWindow)->render();
                //m_pMainWindow->invalidate();
                //SwapBuffers(m_hdc);
            }
        }
        // shutdown OpenGL
        //UGlut::DisableOpenGL( m_hDlg, m_hdc, m_hrc );
        // destroy the window explicitly
        m_pMainWindow->onDestroy();
        return (msg.wParam);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UGLWinApp app;

    app.setMainWindow(new UGLWindow);

    app.init(hInstance);

    return app.run();
}

