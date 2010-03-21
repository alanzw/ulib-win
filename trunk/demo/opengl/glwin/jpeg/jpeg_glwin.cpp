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



class UGLWindow : public UBaseWindow
{
public:
    UGLWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst),
      m_hdc(0), m_hrc(0), m_bFullscreen(FALSE), cnt1(0.0)
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
private:
    HDC m_hdc;
    HGLRC m_hrc;
    UIcon m_uico;

    BOOL m_bFullscreen;

    GLfloat    cnt1;

    UINT TextureArray[1];
private:
    BOOL initGL()
    {
        UGlut::EnableOpenGL(*this, m_hdc, m_hrc);
        
        glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
        glClearDepth(1.0f);									// Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
        glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

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

        glTranslatef(0, 0,-5.0f);

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

        cnt1+=50.f;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    app.setMainWindow(new UGLWindow);

    app.init(hInstance);

    return app.run();
}

