/*
 * =====================================================================================
 *
 *       Filename:  uglut.cpp
 *
 *    Description:  UGlut Functions
 *
 *        Version:  1.0
 *        Created:  2009-8-6 5:58:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>
#include <cstdio>

#include "uglut.h"
#include "umsg.h"

namespace UGlut
{

int EnableOpenGL(HWND hWnd, HDC &hDC, HGLRC &hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    // get the device context (DC)
    hDC = ::GetDC( hWnd );

    // set the pixel format for the DC
    ::ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    if ((iFormat = ChoosePixelFormat(hDC, &pfd))==0)
    {
        return 1;
    }

    if (::SetPixelFormat( hDC, iFormat, &pfd ) == FALSE)
    {
        return 1;
    }

    // create and enable the render context (RC)
    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );

    return 0;
}

int DisableOpenGL(HWND hWnd, HDC &hDC, HGLRC &hRC)
{
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ::ReleaseDC( hWnd, hDC );

    return 0;
}

void ResizeScene(int nWidth, int nHeight)
{
    if (nHeight==0)    // Prevent A Divide By Zero By
    {
        nHeight=1;    // Making Height Equal One
    }

    glViewport(0, 0, nWidth, nHeight); // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);       // Select The Projection Matrix
    glLoadIdentity();                   // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    /*
        gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar) 
    */
    gluPerspective(60.0f,(GLfloat)nWidth/(GLfloat)nHeight, 0.1f, 10.0f);

    glMatrixMode(GL_MODELVIEW);           // Select The Modelview Matrix
    glLoadIdentity();
    

    gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void switchToWireframe()
{
    // Turn on wireframe mode
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
}

void switchToSolid()
{
    // Turn off wireframe mode
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
}


BOOL check_fullscreen(BOOL &flag, int width, int height, int bits)
{
    if (!flag) return TRUE;
    
    DEVMODE dmScreenSettings;                               // Device Mode
    ::ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));    // Makes Sure Memory's Cleared
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // Size Of The Devmode Structure
    dmScreenSettings.dmPelsWidth  = width;                  // Selected Screen Width
    dmScreenSettings.dmPelsHeight = height;                 // Selected Screen Height
    dmScreenSettings.dmBitsPerPel = bits;                   // Selected Bits Per Pixel
    dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

    // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    {
        // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
        if (showYesNoMsgbox(_T("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card.")
                            _T("Use Windowed Mode Instead?"),_T("OpenGL"))==IDYES)
        {
            flag=FALSE;        // Windowed Mode Selected.  Fullscreen = FALSE
        }
        else
        {
            // Pop Up A Message Box Letting User Know The Program Is Closing.
            showMsg(_T("Program Will Now Close."),_T("ERROR"));
            return FALSE;                                    // Return FALSE
        }
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        THE CCAMERA POSITION CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Position_Camera( float pos_x,  float pos_y,  float pos_z,
                                 float view_x, float view_y, float view_z,
                                 float up_x,   float up_y,   float up_z)
{
    UGLVector3 vPos    = UGLVector3(pos_x,  pos_y,  pos_z);
    UGLVector3 vView   = UGLVector3(view_x, view_y, view_z);
    UGLVector3 vUp     = UGLVector3(up_x,   up_y,   up_z);

    mPos  = vPos;                            // set the position
    mView = vView;                           // set the view
    mUp   = vUp;                            // set the up vector
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        THE CCAMERA MOVE CAMERA
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Move_Camera(float cameraspeed)
{
    UGLVector3 vVector = UGLVector3(0,0,0);        // init a new view vector
    vVector = mView - mPos;                    // Get the view vector

    // forward positive cameraspeed and backward negative -cameraspeed.
    mPos.x  = mPos.x  + vVector.x * cameraspeed;
    mPos.z  = mPos.z  + vVector.z * cameraspeed;
    mView.x = mView.x + vVector.x * cameraspeed;
    mView.z = mView.z + vVector.z * cameraspeed;
}


void UGLCamera::Translate_View(float cx, float cy, float cz)
{
    mView.x += cx;
    mView.y += cy;
    mView.z += cz;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        THE CCAMERA ROTATE VIEW
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Rotate_View(float x, float y, float z)
{
    UGLVector3 vVector = mView - mPos;

    if(x)
    {
        mView.z = (float)(mPos.z + sin(x)*vVector.y + cos(x)*vVector.z);
        mView.y = (float)(mPos.y + cos(x)*vVector.y - sin(x)*vVector.z);
    }
    if(y)
    {
        mView.z = (float)(mPos.z + sin(y)*vVector.x + cos(y)*vVector.z);
        mView.x = (float)(mPos.x + cos(y)*vVector.x - sin(y)*vVector.z);
    }
    if(z)
    {
        mView.x = (float)(mPos.x + sin(z)*vVector.y + cos(z)*vVector.x);
        mView.y = (float)(mPos.y + cos(z)*vVector.y - sin(z)*vVector.x);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                        THE CCAMERA MOUSE MOVE
/////////////////////////////////////////////////////////////////////////////////////////////////
void UGLCamera::Mouse_Move()
{
    POINT mousePos;
    int mid_x = mWindowWidth  >> 1;
    int mid_y = mWindowHeight >> 1;
    float angle_y  = 0.0f;
    float angle_z  = 0.0f;

    GetCursorPos(&mousePos);    // Get the 2D mouse cursor (x,y) position

    if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

    SetCursorPos(mid_x, mid_y);    // Set the mouse cursor in the middle of the window

    // Get the direction from the mouse, and bring the number down to a reasonable amount
    angle_y = (float)( (mid_x - mousePos.x) ) / 1000;
    angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

    // The higher the number(acceleration) is the faster the camera looks around.
    mView.y += angle_z * 2;

    // limit the rotation around the x-axis
    if((mView.y - mPos.y) > 8)  mView.y = mPos.y + 8;
    if((mView.y - mPos.y) <-8)  mView.y = mPos.y - 8;

    Rotate_View(0, -angle_y, 0); // Rotate
}


UGLCanvas::UGLCanvas()
{}

bool UGLCanvas::switch_to_fullscreen()
{
    return false;
}

tImageTGA *Load_TGA(const char *strfilename)
{
    tImageTGA *pImgData    = NULL;
    FILE *pFile            = NULL;
    WORD width            = 0;
    WORD height            = 0;
    byte length            = 0;
    byte imgType        = 0;
    byte bits            = 0;
    int channels        = 0;
    int stride            = 0;
    int i                = 0;


    if((pFile = fopen(strfilename, "rb")) == NULL)
    {
        MessageBox(NULL, "Error loading tga file.", "ERROR", MB_OK);
        return NULL;
    }

    pImgData = (tImageTGA*)malloc(sizeof(tImageTGA));

    fread(&length, sizeof(byte), 1, pFile);

    fseek(pFile,1,SEEK_CUR);

    fread(&imgType, sizeof(byte), 1, pFile);

    fseek(pFile, 9, SEEK_CUR);

    fread(&width,  sizeof(WORD), 1, pFile);
    fread(&height, sizeof(WORD), 1, pFile);
    fread(&bits,   sizeof(byte), 1, pFile);

    fseek(pFile, length + 1, SEEK_CUR);

    if(imgType != TGA_RLE)
    {
        // Check for 24 or 32 Bit
        if(bits == 24 || bits == 32)
        {

            channels = bits / 8;
            stride = channels * width;
            pImgData->data = new unsigned char[stride * height];

            for(int y = 0; y < height; y++)
            {
                unsigned char *pLine = &(pImgData->data[stride * y]);

                fread(pLine, stride, 1, pFile);

                for(i = 0; i < stride; i += channels)
                {
                    int temp     = pLine[i];
                    pLine[i]     = pLine[i + 2];
                    pLine[i + 2] = temp;
                }
            }
        }

        // Check for 16 Bit
        else if(bits == 16)
        {
            unsigned short pixels = 0;
            int r=0, g=0, b=0;

            channels = 3;
            stride = channels * width;
            pImgData->data = new unsigned char[stride * height];

            for(int i = 0; i < width*height; i++)
            {
                fread(&pixels, sizeof(unsigned short), 1, pFile);

                b = (pixels & 0x1f) << 3;
                g = ((pixels >> 5) & 0x1f) << 3;
                r = ((pixels >> 10) & 0x1f) << 3;

                pImgData->data[i * 3 + 0] = r;
                pImgData->data[i * 3 + 1] = g;
                pImgData->data[i * 3 + 2] = b;
            }
        }

        else
            return NULL;
    }

    else
    {

        byte rleID = 0;
        int colorsRead = 0;
        channels = bits / 8;
        stride = channels * width;

        pImgData->data = new unsigned char[stride * height];
        byte *pColors = new byte [channels];

        while(i < width*height)
        {

            fread(&rleID, sizeof(byte), 1, pFile);


            if(rleID < 128)
            {
                rleID++;

                while(rleID)
                {
                    fread(pColors, sizeof(byte) * channels, 1, pFile);

                    pImgData->data[colorsRead + 0] = pColors[2];
                    pImgData->data[colorsRead + 1] = pColors[1];
                    pImgData->data[colorsRead + 2] = pColors[0];

                    if(bits == 32)    pImgData->data[colorsRead + 3] = pColors[3];

                    i++;
                    rleID--;
                    colorsRead += channels;
                }
            }

            else
            {
                rleID -= 127;

                fread(pColors, sizeof(byte) * channels, 1, pFile);

                while(rleID)
                {
                    pImgData->data[colorsRead + 0] = pColors[2];
                    pImgData->data[colorsRead + 1] = pColors[1];
                    pImgData->data[colorsRead + 2] = pColors[0];

                    if(bits == 32)    pImgData->data[colorsRead + 3] = pColors[3];

                    i++;
                    rleID--;
                    colorsRead += channels;
                }
            }
        }
        delete[] pColors;
    }

    fclose(pFile);


    pImgData->channels  = channels;
    pImgData->size_x    = width;
    pImgData->size_y    = height;

    return pImgData;
}

void TGA_Texture(UINT textureArray[], LPSTR strFileName, int ID)
{
    if(!strFileName) return;

    tImageTGA *pBitMap = Load_TGA(strFileName);

    if(pBitMap == NULL) return;

    glGenTextures(1, &textureArray[ID]);
    glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
    int textureType = GL_RGB;
    if(pBitMap->channels == 4)    textureType = GL_RGBA;
    
    gluBuild2DMipmaps( GL_TEXTURE_2D,
                       pBitMap->channels, 
                       pBitMap->size_x,
                       pBitMap->size_y,
                       textureType, 
                       GL_UNSIGNED_BYTE,
                       pBitMap->data );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    if (pBitMap)
    {
        if (pBitMap->data)
        {
            delete pBitMap->data;
        }
        delete pBitMap;
    }
}

}; // namespace UGlut

