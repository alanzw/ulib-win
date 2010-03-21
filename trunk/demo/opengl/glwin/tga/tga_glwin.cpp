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

#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

struct tImageTGA
{
	int channels;
	int size_x;	
	int size_y;				
	unsigned char *data;
};

tImageTGA *Load_TGA(const char *strfilename);

void TGA_Texture(UINT textureArray[], LPSTR strFileName, int ID)
{
	if(!strFileName)	return;

	tImageTGA *pBitMap = Load_TGA(strFileName);

	if(pBitMap == NULL)	exit(0);

	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	int textureType = GL_RGB;
	if(pBitMap->channels == 4)	textureType = GL_RGBA;
	gluBuild2DMipmaps(GL_TEXTURE_2D, pBitMap->channels, pBitMap->size_x, pBitMap->size_y, textureType, GL_UNSIGNED_BYTE, pBitMap->data);
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

tImageTGA *Load_TGA(const char *strfilename)
{
	tImageTGA *pImgData	= NULL;		
	FILE *pFile			= NULL;	
	WORD width			= 0;
	WORD height			= 0;			
	byte length			= 0;					
	byte imgType		= 0;					
	byte bits			= 0;						
	int channels		= 0;					
	int stride			= 0;						
	int i				= 0;							
	

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
					
					if(bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];
					
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
					
					if(bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];
					
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
        
        glEnable(GL_TEXTURE_2D);

        TGA_Texture(TextureArray, "texture.tga", 0);

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
        glClear(GL_COLOR_BUFFER_BIT);

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

