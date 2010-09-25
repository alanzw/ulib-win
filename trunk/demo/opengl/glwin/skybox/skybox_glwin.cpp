#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <ctime>
#include <cmath>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "uglut.h"
#include "uicon.h"
#include "umsg.h"

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif // _MSC_VER

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        BMP TEXTURE LOADER
/////////////////////////////////////////////////////////////////////////////////////////////////
void BMP_Texture(UINT textureArray[], LPCTSTR sFileName, int ID)
{
    if(NULL == sFileName) return;

    HBITMAP hBMP;                                                        // Handle Of The Bitmap
    BITMAP bmp;

    hBMP=(HBITMAP)LoadImage( GetModuleHandle(NULL),
                             sFileName,
                             IMAGE_BITMAP,
                             0,
                             0,
                             LR_CREATEDIBSECTION | LR_LOADFROMFILE );

    if (!hBMP)                                                            // Does The Bitmap Exist?
        return;

    ::GetObject(hBMP, sizeof(bmp), &bmp);

    glGenTextures(1, &textureArray[ID]);
    glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_RGB, GL_UNSIGNED_BYTE, bmp.bmBits);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    ::DeleteObject(hBMP);                                                    // Delete The Object
}


#if defined(_BASETSD_H)
 #define _BASETSD_H_
#endif // defined(_BASETSD_H_)

#if defined(__GNUC__) && !defined(HAVE_BOOLEAN)
 #define HAVE_BOOLEAN
#endif

#include "jpeglib.h"

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
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitMap->sizeX, pBitMap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitMap->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

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


#define CAMERASPEED    0.03f                // The Camera Speed

#define SKYFRONT 0                        // Give Front ID = 0
#define SKYBACK  1                        // Give Back  ID = 1
#define SKYLEFT  2                        // Give Left  ID = 2
#define SKYRIGHT 3                        // Give Right ID = 3
#define SKYUP    4                        // Give Up    ID = 4
#define SKYDOWN  5                        // Give Down  ID = 5

class UGLWindow : public UBaseWindow
{
public:
    UGLWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst), m_bFullscreen(FALSE),
    m_hdc(0), m_hrc(0)
    {
        setTitle(_T("OpenGL Window"));
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


        this->setIconBig(IDI_APP);

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
        //SwapBuffers(m_hdc);
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        if(wParam == 'w' || wParam == 'W')
        {
            objCamera.Move_Camera(CAMERASPEED);
            LOG_STRING("!!!Key Up!!!");
            return FALSE;
        }

        if(wParam == 's' || wParam == 'S')
        {
            objCamera.Move_Camera(-CAMERASPEED);
            return FALSE;
        }

        if(wParam == 'a' || wParam == 'A')
        {
            objCamera.Rotate_View(0,-CAMERASPEED, 0);
            return FALSE;
        }

        if(wParam == 'd' || wParam == 'D')
        {
            objCamera.Rotate_View(0, CAMERASPEED, 0);
            return FALSE;
        }

        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

private:
    HDC m_hdc;
    HGLRC m_hrc;
    UIcon m_uico;

    BOOL m_bFullscreen;

    UINT SkyboxTexture[6];        // We need 6 textures for our Skybox
    UGlut::UGLCamera objCamera;
private:
    BOOL initGL()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);

        RECT rc = {0};
        this->getClientRect(&rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;

        UGlut::ResizeScene(w, h);

        objCamera.mWindowWidth  = w;
        objCamera.mWindowHeight = h;
        //glMatrixMode(GL_PROJECTION);
        //glLoadIdentity();
        //gluPerspective(60.0, (float) w / (float) h, 0.1, 100.0);

        //glMatrixMode(GL_MODELVIEW);
        //glViewport(0, 0, w, h);

        glShadeModel(GL_SMOOTH);
        glClearColor(0.0f, 0.0f, 0.0f, 0.f);                // Black Background
        glClearDepth(1.0f);                                // Depth Buffer Setup

    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

    glEnable(GL_TEXTURE_2D);

                           // Position      View(target)  Up
    objCamera.Position_Camera(0, 2.5f, 5,    0, 2.5f, 0,   0, 1, 0);

    // Load the Skybox textures
    //BMP_Texture(SkyboxTexture,"texture/front.bmp",  SKYFRONT);
    //BMP_Texture(SkyboxTexture,"texture/back.bmp",   SKYBACK);
    //BMP_Texture(SkyboxTexture,"texture/left.bmp",   SKYLEFT);
    //BMP_Texture(SkyboxTexture,"texture/right.bmp",  SKYRIGHT);
    //BMP_Texture(SkyboxTexture,"texture/up.bmp",     SKYUP);
    //BMP_Texture(SkyboxTexture,"texture/down.bmp",   SKYDOWN);

    JPEG_Texture(SkyboxTexture,"texture/front.jpg",  SKYFRONT);
    JPEG_Texture(SkyboxTexture,"texture/back.jpg",   SKYBACK);
    JPEG_Texture(SkyboxTexture,"texture/left.jpg",   SKYLEFT);
    JPEG_Texture(SkyboxTexture,"texture/right.jpg",  SKYRIGHT);
    JPEG_Texture(SkyboxTexture,"texture/up.jpg",     SKYUP);
    JPEG_Texture(SkyboxTexture,"texture/down.jpg",   SKYDOWN);

        return TRUE;
    }

public:
    void render()
    {
    /*
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float) 800 / (float) 600, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
      */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear Screen And Depth Buffer
    glLoadIdentity();                                    // Reset The Current Modelview Matrix

//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

        // use this function for opengl target camera
        gluLookAt(objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,
              objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
              objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);


        Draw_Grid();                    // Draw grid

        Draw_Skybox(0,0,0,100,100,100);    // Draw the Skybox
        //
        SwapBuffers(m_hdc);
    }

    void Draw_Grid()
    {

        for(float i = -500; i <= 500; i += 5)
        {
            glBegin(GL_LINES);
                glColor3ub(150, 190, 150);
                glVertex3f(-500, 0, i);
                glVertex3f(500, 0, i);
                glVertex3f(i, 0,-500);
                glVertex3f(i, 0, 500);
            glEnd();
        }
    }

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
    // Center the Skybox around the given x,y,z position
    x = x - width  / 2;
    y = y - height / 2;
    z = z - length / 2;


    // Draw Front side
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,          y,        z+length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,          y+height, z+length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,        z+length);
    glEnd();

    // Draw Back side
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,        z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,          y+height,    z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,          y,        z);
    glEnd();

    // Draw Left side
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYLEFT]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,          y+height,    z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,          y+height,    z+length);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,          y,        z+length);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,          y,        z);
    glEnd();

    // Draw Right side
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYRIGHT]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,        z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,        z+length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,    z+length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,    z);
    glEnd();

    // Draw Up side
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYUP]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x,          y+height,    z+length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,          y+height,    z);
    glEnd();

    // Draw Down side
    glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYDOWN]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,          y,        z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x,          y,        z+length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,        z+length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,        z);
    glEnd();

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

