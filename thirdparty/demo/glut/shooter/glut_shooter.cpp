/*
First Person Shooter - A 3D OpenGL First Person Shooter Demo
Copyright (C) 2002  Steve Wortham

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation version 2

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
 *   First Person Shooter
 *   by: Steve Wortham
 *   E-Mail: steve@gldomain.com
 *   website: www.gldomain.com
 *
 *     last updated 4/12/2002
 */
#define WIN32_LEAN_AND_MEAN

#include <math.h>            // Header File For Math functions
#include <windows.h>        // Header File For Windows
#include <stdio.h>            // Header File For Standard Input/Output
#include <gl\gl.h>            // Header File For The OpenGL32 Library
#include <gl\glu.h>            // Header File For The GLu32 Library
#include <gl\glut.h>
#include "glext.h"            // Header File For Multitexturing
#include "sound.h"            // Header File For a Customized Sound Library

HDC        hDC=NULL;            // Private GDI Device Context
HGLRC    hRC=NULL;            // Permanent Rendering Context
HWND    hWnd=NULL;            // Holds Our Window Handle

#define    MAX_PARTICLES    45
#define    MAX_SHOTS_FIRED    45
DirectSoundBuffer Sound1[MAX_SHOTS_FIRED];
DirectSoundBuffer Sound2[MAX_SHOTS_FIRED];
DirectSoundManager SoundManager;

GLdouble WIDTH;
GLdouble HEIGHT;

GLdouble    mouse_x, mouse_y;
GLdouble    mouse_3d_x, mouse_3d_y;
int mouse_wrap_x = 0;
int mouse_wrap_y = 0;
bool invert_mouse = false;  // Used to invert the y-axis for the mouse
GLdouble sensitivity = 8;  // the higher the #, the lower mouse the sensitivity.

bool    keys[256];            // Array Used For The Keyboard Routine
bool  fp;                    // Temporary variable used in association with the isFPS variable
bool  ip;                    // Temporary variable used in association with the invert_mouse variable

bool active=TRUE;        // Window Active Flag Set To TRUE By Default
bool fullscreen=TRUE;    // Fullscreen Flag Set To Fullscreen Mode By Default
bool isFPS=FALSE;
bool GenerateRadiosity=FALSE;
bool is_m16=TRUE;
float m16_kick = 0;
float m16_kicki = 0;
bool is_shock_rifle=FALSE;
bool is_shock_rifle_blast=FALSE;

bool isFire = false;
bool isFireComplete = true;
bool isRicochet = false;
GLdouble BlankNum = 0;

BOOL done=FALSE;                                // Bool Variable To Exit Loop

GLdouble shock_rifle_blast_life = 1;
GLdouble shock_rifle_blast_life2 = 1;
GLdouble shock_rifle_blast_life3 = .5f;
GLdouble shock_rifle_blast_life3i = .015f;
GLdouble shock_rifle_blast_size = .1f;
int shock_rifle_blast_delay = 0;

int loop;
GLdouble V;
GLdouble Angle;
GLdouble AngleZ;

GLUquadricObj *quadratic;    // Storage For Our Quadratic Objects ( NEW )
GLuint    texture[17];

// Absolute value function(comes in handy later).
inline GLdouble ABS(GLdouble A)
{
  if (A < 0)
  A = -A;
  return A;
}
// Hypotenuse Function
inline GLdouble Hypot(GLdouble a, GLdouble b)
{
  return sqrt((a*a)+(b*b));
}

LRESULT    CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);    // Declaration For WndProc
void KillGLWindow(void);


/*    Here Comes The ARB-Multitexture Support.
 *    There Are (Optimally) 6 New Commands To The OpenGL Set:
 *    glMultiTexCoordifARB i=1..4    : Sets Texture-Coordinates For Texel-Pipeline #i
 *    glActiveTextureARB            : Sets Active Texel-Pipeline
 *    glClientActiveTextureARB    : Sets Active Texel-Pipeline For The Pointer-Array-Commands
 *
 *    There Are Even More For The Various Formats Of glMultiTexCoordi{f,fv,d,i}, But We Don't Need Them.
 */

#define __ARB_ENABLE true                                            // Used To Disable ARB Extensions Entirely

// #define EXT_INFO                                                    // Do You Want To See Your Extensions At Start-Up?
#define MAX_EXTENSION_SPACE 10240                                    // Characters for Extension-Strings
#define MAX_EXTENSION_LENGTH 256                                    // Maximum Of Characters In One Extension-String
bool multitextureSupported=false;                                    // Flag Indicating Whether Multitexturing Is Supported
bool useMultitexture=true;                                            // Use It If It Is Supported?
GLint maxTexelUnits=1;                                                // Number Of Texel-Pipelines. This Is At Least 1.

PFNGLMULTITEXCOORD1FARBPROC        glMultiTexCoord1fARB    = NULL;
PFNGLMULTITEXCOORD2FARBPROC        glMultiTexCoord2fARB    = NULL;
PFNGLMULTITEXCOORD3FARBPROC        glMultiTexCoord3fARB    = NULL;
PFNGLMULTITEXCOORD4FARBPROC        glMultiTexCoord4fARB    = NULL;
PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB        = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC    glClientActiveTextureARB= NULL;

// Always Check For Extension-Availability During Run-Time!
// Here We Go!
bool isInString(char *string, const char *search) {
    int pos=0;
    int maxpos=strlen(search)-1;
    int len=strlen(string);
    char *other;
    for (int i=0; i<len; i++) {
        if ((i==0) || ((i>1) && string[i-1]=='\n')) {                // New Extension Begins Here!
            other=&string[i];
            pos=0;                                                    // Begin New Search
            while (string[i]!='\n') {                                // Search Whole Extension-String
                if (string[i]==search[pos]) pos++;                    // Next Position
                if ((pos>maxpos) && string[i+1]=='\n') return true; // We Have A Winner!
                i++;
            }
        }
    }
    return false;                                                    // Sorry, Not Found!
}

// isMultitextureSupported() Checks At Run-Time If Multitexturing Is Supported
bool initMultitexture(void) {
    char *extensions;
    extensions=strdup((char *) glGetString(GL_EXTENSIONS));            // Fetch Extension String
    int len=strlen(extensions);
    for (int i=0; i<len; i++)                                        // Separate It By Newline Instead Of Blank
        if (extensions[i]==' ') extensions[i]='\n';

#ifdef EXT_INFO
    MessageBox(hWnd,extensions,"supported GL extensions",MB_OK | MB_ICONINFORMATION);
#endif

    if (isInString(extensions,"GL_ARB_multitexture")                // Is Multitexturing Supported?
        && __ARB_ENABLE)                                                // Override-Flag
    {
        glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
        glMultiTexCoord1fARB    = (PFNGLMULTITEXCOORD1FARBPROC)        wglGetProcAddress("glMultiTexCoord1fARB");
        glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)        wglGetProcAddress("glMultiTexCoord2fARB");
        glMultiTexCoord3fARB    = (PFNGLMULTITEXCOORD3FARBPROC)        wglGetProcAddress("glMultiTexCoord3fARB");
        glMultiTexCoord4fARB    = (PFNGLMULTITEXCOORD4FARBPROC)        wglGetProcAddress("glMultiTexCoord4fARB");
        glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)        wglGetProcAddress("glActiveTextureARB");
        glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)    wglGetProcAddress("glClientActiveTextureARB");
#ifdef EXT_INFO
    MessageBox(hWnd,"The GL_ARB_multitexture extension will be used.","feature supported!",MB_OK | MB_ICONINFORMATION);
#endif
        return true;
    }
    return false;
}

typedef struct                                                // Create A Structure
{
    GLubyte    *imageData;                                        // Image Data (Up To 32 Bits)
    GLuint    bpp;                                            // Image Color Depth In Bits Per Pixel.
    GLuint    width;                                            // Image Width
    GLuint    height;                                            // Image Height
    GLuint    texID;                                            // Texture ID Used To Select A Texture
} TextureImage;                                                // Structure Name

GLuint        base;                                            // Base Display List For The Font
TextureImage textures[2];                                    // Storage For One Texture


bool LoadTGA(TextureImage *texture, char *filename)            // Loads A TGA File Into Memory
{
    GLubyte        TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};    // Uncompressed TGA Header
    GLubyte        TGAcompare[12];                                // Used To Compare TGA Header
    GLubyte        header[6];                                    // First 6 Useful Bytes From The Header
    GLuint        bytesPerPixel;                                // Holds Number Of Bytes Per Pixel Used In The TGA File
    GLuint        imageSize;                                    // Used To Store The Image Size When Setting Aside Ram
    GLuint        temp;                                        // Temporary Variable
    GLuint        type=GL_RGBA;                                // Set The Default GL Mode To RBGA (32 BPP)

    FILE *file = fopen(filename, "rb");                        // Open The TGA File

    if(    file==NULL ||                                        // Does File Even Exist?
        fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||    // Are There 12 Bytes To Read?
        memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0                ||    // Does The Header Match What We Want?
        fread(header,1,sizeof(header),file)!=sizeof(header))                // If So Read Next 6 Header Bytes
    {
        if (file == NULL)                                    // Did The File Even Exist? *Added Jim Strong*
            return false;                                    // Return False
        else
        {
            fclose(file);                                    // If Anything Failed, Close The File
            return false;                                    // Return False
        }
    }

    texture->width  = header[1] * 256 + header[0];            // Determine The TGA Width    (highbyte*256+lowbyte)
    texture->height = header[3] * 256 + header[2];            // Determine The TGA Height    (highbyte*256+lowbyte)

     if(    texture->width    <=0    ||                                // Is The Width Less Than Or Equal To Zero
        texture->height    <=0    ||                                // Is The Height Less Than Or Equal To Zero
        (header[4]!=24 && header[4]!=32))                    // Is The TGA 24 or 32 Bit?
    {
        fclose(file);                                        // If Anything Failed, Close The File
        return false;                                        // Return False
    }

    texture->bpp    = header[4];                            // Grab The TGA's Bits Per Pixel (24 or 32)
    bytesPerPixel    = texture->bpp/8;                        // Divide By 8 To Get The Bytes Per Pixel
    imageSize        = texture->width*texture->height*bytesPerPixel;    // Calculate The Memory Required For The TGA Data

    texture->imageData=(GLubyte *)malloc(imageSize);        // Reserve Memory To Hold The TGA Data

    if(    texture->imageData==NULL ||                            // Does The Storage Memory Exist?
        fread(texture->imageData, 1, imageSize, file)!=imageSize)    // Does The Image Size Match The Memory Reserved?
    {
        if(texture->imageData!=NULL)                        // Was Image Data Loaded
            free(texture->imageData);                        // If So, Release The Image Data

        fclose(file);                                        // Close The File
        return false;                                        // Return False
    }

    for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)        // Loop Through The Image Data
    {                                                        // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
        temp=texture->imageData[i];                            // Temporarily Store The Value At Image Data 'i'
        texture->imageData[i] = texture->imageData[i + 2];    // Set The 1st Byte To The Value Of The 3rd Byte
        texture->imageData[i + 2] = temp;                    // Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
    }

    fclose (file);                                            // Close The File

    // Build A Texture From The Data
    glGenTextures(1, &texture[0].texID);                    // Generate OpenGL texture IDs

    glBindTexture(GL_TEXTURE_2D, texture[0].texID);            // Bind Our Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // Linear Filtered
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    // Linear Filtered

    if (texture[0].bpp==24)                                    // Was The TGA 24 Bits
    {
        type=GL_RGB;                                        // If So Set The 'type' To GL_RGB
    }

       gluBuild2DMipmaps(GL_TEXTURE_2D, type, texture[0].width, texture[0].height, type, GL_UNSIGNED_BYTE, texture[0].imageData);

    return true;                                            // Texture Building Went Ok, Return True
}

void BuildFont(void)                                    // Build Our Font Display List
{
    base=glGenLists(256);                                    // Creating 256 Display Lists
    glBindTexture(GL_TEXTURE_2D, textures[0].texID);        // Select Our Font Texture
    for (int loop1=0; loop1<256; loop1++)                    // Loop Through All 256 Lists
    {
        float cx=float(loop1%16)/16.0f;                        // X Position Of Current Character
        float cy=float(loop1/16)/16.0f;                        // Y Position Of Current Character

        glNewList(base+loop1,GL_COMPILE);                    // Start Building A List
            glBegin(GL_QUADS);                                // Use A Quad For Each Character
                glTexCoord2f(cx,1.0f-cy-0.0625f);            // Texture Coord (Bottom Left)
                glVertex2d(0,16);                            // Vertex Coord (Bottom Left)
                glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);    // Texture Coord (Bottom Right)
                glVertex2i(16,16);                            // Vertex Coord (Bottom Right)
                glTexCoord2f(cx+0.0625f,1.0f-cy-0.001f);    // Texture Coord (Top Right)
                glVertex2i(16,0);                            // Vertex Coord (Top Right)
                glTexCoord2f(cx,1.0f-cy-0.001f);            // Texture Coord (Top Left)
                glVertex2i(0,0);                            // Vertex Coord (Top Left)
            glEnd();                                        // Done Building Our Quad (Character)
            glTranslated(14,0,0);                            // Move To The Right Of The Character
        glEndList();                                        // Done Building The Display List
    }                                                        // Loop Until All 256 Are Built
}

void KillFont(void)                                        // Delete The Font From Memory
{
    glDeleteLists(base,256);                                // Delete All 256 Display Lists
}

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)    // Where The Printing Happens
{
    char        text[1024];                                    // Holds Our String
    va_list        ap;                                            // Pointer To List Of Arguments

    if (fmt == NULL)                                        // If There's No Text
        return;                                                // Do Nothing

    va_start(ap, fmt);                                        // Parses The String For Variables
        vsprintf(text, fmt, ap);                            // And Converts Symbols To Actual Numbers
    va_end(ap);                                                // Results Are Stored In Text

    if (set>1)                                                // Did User Choose An Invalid Character Set?
    {
        set=1;                                                // If So, Select Set 1 (Italic)
    }

    glEnable(GL_TEXTURE_2D);                                // Enable Texture Mapping
    glLoadIdentity();                                        // Reset The Modelview Matrix
    glTranslated(x,y,-500);                                    // Position The Text (0,0 - Top Left)
    glRotatef(180,0,0,1);
    glRotatef(180,0,1,0);
    glListBase(base-32+(128*set));                            // Choose The Font Set (0 or 1)

    glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);        // Write The Text To The Screen
    glDisable(GL_TEXTURE_2D);                                // Disable Texture Mapping
}

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

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);         // Pixel Storage Mode (Word Alignment / 4 Bytes)


	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_RGB, GL_UNSIGNED_BYTE, bmp.bmBits);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	::DeleteObject(hBMP);                                                    // Delete The Object
}

// Create A Structure For The Timer Information
struct
{
  __int64       frequency;          // Timer Frequency
  GLdouble            resolution;          // Timer Resolution
  unsigned long mm_timer_start;

  // Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;      // Multimedia Timer Elapsed Time
  bool   performance_timer;

  // Using The Performance Timer?
  __int64       performance_timer_start;      // Performance Timer Start Value
  __int64       performance_timer_elapsed; // Performance Timer Elapsed Time
} timer;

// Initialize Our Timer
void TimerInit(void)
{
     memset(&timer, 0, sizeof(timer));
 // Clear Our Timer Structure
     // Check To See If A Performance Counter Is Available
     // If One Is Available The Timer Frequency Will Be Updated
     if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
     {
          // No Performace Counter Available
          timer.performance_timer = FALSE;                      // Set Performance Timer To FALSE
          timer.mm_timer_start = timeGetTime();                 // Use timeGetTime()
          timer.resolution  = 1.0f/1000.0f;                           // Set Our Timer Resolution To .001f
          timer.frequency   = 1000;                                     // Set Our Timer Frequency To 1000
          timer.mm_timer_elapsed = timer.mm_timer_start; // Set The Elapsed Time
     }
     else
     {
          // Performance Counter Is Available, Use It Instead Of The Multimedia Timer
          // Get The Current Time And Store It In performance_timer_start
          QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
          timer.performance_timer   = TRUE;    // Set Performance Timer To TRUE
          // Calculate The Timer Resolution Using The Timer Frequency
          timer.resolution    = (GLdouble) (((double)1.0f)/((double)timer.frequency));
          // Set The Elapsed Time To The Current Time
          timer.performance_timer_elapsed = timer.performance_timer_start;
     }
}

// Get Time In Milliseconds
inline GLdouble TimerGetTime()
{
  __int64 time;                                  // 'time' Will Hold A 64 Bit Integer
  if (timer.performance_timer)           // Are We Using The Performance Timer?
  {
    QueryPerformanceCounter((LARGE_INTEGER *) &time); // Current Performance Time
    // Return The Time Elapsed since TimerInit was called
    return ( (GLdouble) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
  }
  else
  {
    // Return The Time Elapsed since TimerInit was called
    return ( (GLdouble) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
  }
}


int LoadGLTextures()                                    // Load Bitmap And Convert To A Texture
{
  //if (TextureImage[0]=LoadBMP("texture/floor.bmp"))    // Load Particle Texture

	BMP_Texture(texture, "texture/floor.bmp", 0);

  //if (texture[0]=LoadBMP("texture/wall.bmp"))    // Load Particle Texture
	BMP_Texture(texture, "texture/wall.bmp", 1);


  //if (texture[0]=LoadBMP("texture/clay.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/clay.bmp", 2);


  //if (texture[0]=LoadBMP("texture/LightMapFloor.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/LightMapFloor.bmp", 3);


  //if (texture[0]=LoadBMP("texture/LightMapCeiling.bmp"))    // Load Particle Texture
	BMP_Texture(texture, "texture/LightMapCeiling.bmp", 4);


  //if (texture[0]=LoadBMP("texture/LightMapBackWall.bmp"))    // Load Particle Texture
	BMP_Texture(texture, "texture/LightMapBackWall.bmp", 5);


  //if (texture[0]=LoadBMP("texture/LightMapFrontWall.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/LightMapFrontWall.bmp", 6);


  //if (texture[0]=LoadBMP("texture/LightMapLeftWall.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/LightMapLeftWall.bmp", 7);

  //if (texture[0]=LoadBMP("texture/LightMapRightWall.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/LightMapRightWall.bmp", 8);


  //if (texture[0]=LoadBMP("texture/clouds.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/clouds.bmp", 9);


  //if (texture[0]=LoadBMP("texture/m16.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/m16.bmp", 10);


  //if (texture[0]=LoadBMP("texture/shock_rifle.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/shock_rifle.bmp", 11);


  //if (texture[0]=LoadBMP("texture/shock_rifle_blast.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/shock_rifle_blast.bmp", 12);


  //if (texture[0]=LoadBMP("texture/shock_rifle_blast2.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/shock_rifle_blast2.bmp", 13);


  //if (texture[0]=LoadBMP("texture/crosshair.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/crosshair.bmp", 14);

  //if (texture[0]=LoadBMP("texture/smoke.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/smoke.bmp", 15);


  //if (texture[0]=LoadBMP("texture/ceiling.bmp"))    // Load Particle Texture
  BMP_Texture(texture, "texture/ceiling.bmp", 16);

  return 0;                                    // Return The Status
}


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)        // Resize And Initialize The GL Window
{
    if (height==0)                                        // Prevent A Divide By Zero By
    {
        height=1;                                        // Making Height Equal One
    }

    WIDTH=width;
    HEIGHT=height;
    glViewport(0,0,width,height);                        // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    glLoadIdentity();                                    // Reset The Modelview Matrix
    gluPerspective(60.0f,(GLdouble)width/(GLdouble)height,1.0f,1250.0f);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}

static GLuint    ROOM;                // Storage For The Room Display List

const int MAX_Cells = 256;
GLdouble xtrans2 = MAX_Cells/3;
GLdouble ytrans2 = 7;
GLdouble ztrans2 = MAX_Cells/1.5f;

GLdouble xtrans = 0;
GLdouble xptrans = 0;
GLdouble ztrans = 0;
GLdouble zptrans = 0;

// Build Cube Display Lists
GLvoid BuildLists()
{
  if (multitextureSupported)
  {
    ROOM=glGenLists(1);                                    // Generate 2 Different Lists
    glNewList(ROOM,GL_COMPILE);                            // Start With The Box List

    //glEnable(GL_BLEND);

    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    // TEXTURE-UNIT #0
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    // TEXTURE-UNIT #1:
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);

    /////////////////
    // CEILING & FLOOR
    // --------------

    glColor4d(1,1,1,1.f);
    // texture[1] and texture[4] // Floor texture
    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,10, 10);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1);
    glVertex3d(MAX_Cells,0,MAX_Cells);

      glMultiTexCoord2fARB(GL_TEXTURE0_ARB,10, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0);
    glVertex3d(MAX_Cells,0,0);

      glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0);
      glVertex3d(0,0,0);

      glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 10);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1);
    glVertex3d(0,0,MAX_Cells);
    glEnd();

    // TEXTURE-UNIT #0
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[17]);
    // TEXTURE-UNIT #1:
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);

    // texture[1] and texture[5] // Ceiling texture
    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,10, 10);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1);
    glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,10, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0);
    glVertex3d(MAX_Cells,MAX_Cells/4,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0);
    glVertex3d(0,MAX_Cells/4,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 10);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1);
    glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glEnd();

    /////////////////
    // WALLS
    // --------------

    // TEXTURE-UNIT #0
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    // TEXTURE-UNIT #1:
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);

    // texture[2] and texture[6] // Back Wall texture

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1);
    glVertex3d(MAX_Cells,MAX_Cells/4,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0);
    glVertex3d(MAX_Cells,0,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0);
    glVertex3d(0,0,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1);
    glVertex3d(0,MAX_Cells/4,0);
    glEnd();


    // TEXTURE-UNIT #1:
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    // texture[2] and texture[7] // Front Wall texture

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1);
    glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0);
    glVertex3d(MAX_Cells,0,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0);
    glVertex3d(0,0,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1);
    glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glEnd();

    // TEXTURE-UNIT #1:
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[8]);

    // texture[2] and texture[8] // Left Wall texture

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1);
    glVertex3d(0,MAX_Cells/4,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0);
    glVertex3d(0,0,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0);
    glVertex3d(0,0,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1);
    glVertex3d(0,MAX_Cells/4,0);
    glEnd();


    // TEXTURE-UNIT #1:
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[9]);

    // texture[2] and texture[9] // Right Wall texture

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 1);
    glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,4, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,1, 0);
    glVertex3d(MAX_Cells,0,MAX_Cells);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 0);
    glVertex3d(MAX_Cells,0,0);

    glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB,0, 1);
    glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glEnd();

    //FINISH MULTITEXTURING
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    //FINISH MULTITEXTURING

    // DRAW CENTERPIECE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glColor4d (1,1,1,1);

    glBegin(GL_QUADS);
    glTexCoord2d(2,2);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2+26);
    glTexCoord2d(2,0);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2-26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2-26);
    glTexCoord2d(0,2);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2+26);
    glEnd();

    glColor4d (.75f,.75f,.75f,1);

    glBegin(GL_QUADS);
    glTexCoord2d(2,.25f);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2-26);
    glTexCoord2d(2,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2-26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2-26);
    glTexCoord2d(0,.25f);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2-26);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(2,.25f);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2+26);
    glTexCoord2d(2,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2+26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2+26);
    glTexCoord2d(0,.25f);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2+26);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(2,.25f);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2+26);
    glTexCoord2d(2,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2+26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2-26);
    glTexCoord2d(0,.25f);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2-26);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(2,.25f);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2+26);
    glTexCoord2d(2,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2+26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2-26);
    glTexCoord2d(0,.25f);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2-26);
    glEnd();

    glColor4d (.2f,.2f,.2f,1);

    glBegin(GL_QUADS);
    glTexCoord2d(2,2);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2+26);
    glTexCoord2d(2,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2-26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2-26);
    glTexCoord2d(0,2);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2+26);
    glEnd();
    glEnable(GL_BLEND);

    glEndList();
  }

  else
  {
    ROOM=glGenLists(1);                                    // Generate 2 Different Lists
    glNewList(ROOM,GL_COMPILE);                            // Start With The Box List

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glColor4d(1,1,1,1.f);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glBegin(GL_QUADS);
    glTexCoord2d(5,5);  glVertex3d(MAX_Cells,0,MAX_Cells);
    glTexCoord2d(5,0);  glVertex3d(MAX_Cells,0,0);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,5);  glVertex3d(0,0,MAX_Cells);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[17]);

    glBegin(GL_QUADS);
    glTexCoord2d(5,5);  glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(5,0);  glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glTexCoord2d(0,0);  glVertex3d(0,MAX_Cells/4,0);
    glTexCoord2d(0,5);  glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glEnd();


    glBlendFunc(GL_DST_COLOR,GL_ZERO);                    // Type Of Blending To Perform
    glColor4d(1,1,1,.25f);

    glBindTexture(GL_TEXTURE_2D, texture[4]);

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells,0,MAX_Cells);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells,0,0);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,0,MAX_Cells);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glTexCoord2d(0,0);  glVertex3d(0,MAX_Cells/4,0);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glEnd();


       glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glColor4d(1,1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2d(4,1);  glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glTexCoord2d(4,0);  glVertex3d(MAX_Cells,0,0);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,0);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(4,1);  glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(4,0);  glVertex3d(MAX_Cells,0,MAX_Cells);
    glTexCoord2d(0,0);  glVertex3d(0,0,MAX_Cells);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(4,1);  glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(4,0);  glVertex3d(0,0,MAX_Cells);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,0);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(4,1);  glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(4,0);  glVertex3d(MAX_Cells,0,MAX_Cells);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells,0,0);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glEnd();


    glBlendFunc(GL_DST_COLOR,GL_ZERO);                    // Type Of Blending To Perform
    glColor4d(1,1,1,.25f);

    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells,0,0);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells,0,MAX_Cells);
    glTexCoord2d(0,0);  glVertex3d(0,0,MAX_Cells);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(0,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(1,0);  glVertex3d(0,0,MAX_Cells);
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(0,1);  glVertex3d(0,MAX_Cells/4,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells,MAX_Cells/4,MAX_Cells);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells,0,MAX_Cells);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells,0,0);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells,MAX_Cells/4,0);
    glEnd();


    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glColor4d (1,1,1,1);

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2+26);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2-26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2-26);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2+26);
    glEnd();

    glColor4d (.75f,.75f,.75f,1);

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2-26);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2-26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2-26);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2-26);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2+26);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2+26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2+26);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2+26);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2+26);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2+26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2-26);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells/2-26,8,MAX_Cells/2-26);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2+26);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2+26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2-26);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells/2+26,8,MAX_Cells/2-26);
    glEnd();

    glColor4d (.2f,.2f,.2f,1);

    glBegin(GL_QUADS);
    glTexCoord2d(1,1);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2+26);
    glTexCoord2d(1,0);  glVertex3d(MAX_Cells/2+26,6,MAX_Cells/2-26);
    glTexCoord2d(0,0);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2-26);
    glTexCoord2d(0,1);  glVertex3d(MAX_Cells/2-26,6,MAX_Cells/2+26);
    glEnd();

    glEndList();
  }

}

GLdouble Time1;
GLdouble Time2;
GLdouble DiffTime;
GLdouble StartTime;

int InitGL(void)                                        // All Setup For OpenGL Goes Here
{
	int i;

    if (!LoadTGA(&textures[0],"texture/Font.TGA"))                // Load The Font Texture
    {
        return false;                                        // If Loading Failed, Return False
    }

    multitextureSupported=initMultitexture();

    if (!SoundManager.Initialize(hWnd))
    {
        return false;
    }

    for ( i = 0; i < MAX_SHOTS_FIRED; i++)
    {
      SoundManager.CreateSound("rifle.wav", Sound1[i]);
      Sound1[i].SetFrequency(44100);
    }

    for (i = 0; i < MAX_SHOTS_FIRED; i++)
    {
      SoundManager.CreateSound("laser.wav", Sound2[i]);
    }

    LPARAM lParam;
    SetCursorPos(322,240);
    float temp_mouse_x = LOWORD(lParam);
    float temp_mouse_y = HIWORD(lParam);
    SetCursorPos(320,240);

    if (!LoadGLTextures())                                // Jump To Texture Loading Routine
      return false;
  BuildLists();

  glEnable(GL_TEXTURE_2D);                    // Enable Texture Mapping
    glClearColor(0, 0, 0, 1.0f);                // Black Background
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  TimerInit(); //initialize timer
  BuildFont();
  StartTime = TimerGetTime()/1000;
    return TRUE;                                        // Initialization Went OK
}

GLdouble    yrot=0;                // Y Rotation

const GLdouble piover180 = 0.0174532925f;
GLdouble XP=0;
GLdouble ZP=0;

GLdouble sceneroty;
GLdouble heading;
GLdouble _heading = 0;
GLdouble walkbias = 0;
GLdouble walkbiasangle = 0;
GLdouble zprot;

GLdouble XPOS = (GLdouble)(MAX_Cells/2);
GLdouble ZPOS = (GLdouble)(MAX_Cells/2);

GLdouble size = .25f;
int frames = 0;

GLdouble FPS = 0;

GLdouble fire_x = 0;
GLdouble fire_y = 10;
GLdouble fire_xp, fire_yp;
GLdouble fire_z = 0;
GLdouble fire_zp = 0;
GLdouble fire_delay = 0;

int shots_fired = 0;

typedef struct                        // Create A Structure For Particle
{
    GLdouble    life;                    // Particle Life
    GLdouble    fade;                    // Fade Speed
    GLdouble    size;                    // Fade Speed
    GLdouble    triangle_x1;                        // X vertex
    GLdouble    triangle_x2;                        // X vertex
    GLdouble    triangle_x3;                        // X vertex
    GLdouble    triangle_y1;                        // Y vertex
    GLdouble    triangle_y2;                        // Y vertex
    GLdouble    triangle_y3;                        // Y vertex
    GLdouble    triangle_z1;                        // Z vertex
    GLdouble    triangle_z2;                        // Z vertex
    GLdouble    triangle_z3;                        // Z vertex
    GLdouble    triangle_rotate_x;                    // X Rotate
    GLdouble    triangle_rotate_y;                    // X Rotate
    GLdouble    triangle_rotate_z;                    // X Rotate
    GLdouble    triangle_rotate_xi;                    // X Rotate
    GLdouble    triangle_rotate_yi;                    // X Rotate
    GLdouble    triangle_rotate_zi;                    // X Rotate
    GLdouble    x;                        // X Position
    GLdouble    y;                        // Y Position
    GLdouble    z;                        // Z Position
    GLdouble    xi;                        // X Direction
    GLdouble    yi;                        // Y Direction
    GLdouble    zi;                        // Z Direction
}
particles;                            // Particles Structure

particles particle[MAX_PARTICLES][MAX_SHOTS_FIRED];    // Particle Array (Room For Particle Info)

// Detect collision and reflect movement if collision is detected
// cx = x position; cy = y position; cz = z position;
// cxi = x increment; cyi = y increment; czi = z increment;
// padding = a square border around the object.  your value will be 1/2 the length of one side of the square
// bounce = bounce coefficient -- 0 = no bounce; 1 = perfect bounce
inline bool DetectCollision(GLdouble &cx, GLdouble &cy, GLdouble &cz, GLdouble &cxi, GLdouble &cyi, GLdouble &czi, GLdouble padding, GLdouble bounce)
{
  bool Status = false;
  if (cx > MAX_Cells-padding)
  {
    cx = MAX_Cells-padding;
    cxi = -cxi;
    cxi *= bounce;
    Status = true;
  }
  else if (cx < 0+padding)
  {
    cx = 0+padding;
    cxi = -cxi;
    cxi *= bounce;
    Status = true;
  }

  if (cy > MAX_Cells/4-padding)
  {
    cy = MAX_Cells/4-padding;
    cyi = -cyi;
    cyi *= bounce;
    Status = true;
  }
  else if (cy < 0+padding)
  {
    cy = 0+padding;
    cyi = -cyi;
    cyi *= bounce;
    Status = true;
  }

  if (cz > MAX_Cells-padding)
  {
    cz = MAX_Cells-padding;
    czi = -czi;
    czi *= bounce;
    Status = true;
  }
  else if (cz < 0+padding)
  {
    cz = 0+padding;
    czi = -czi;
    czi *= bounce;
    Status = true;
  }

  if (cx < MAX_Cells/2+26+padding && cx > MAX_Cells/2-26-padding && cz < MAX_Cells/2+26+padding && cz > MAX_Cells/2-26-padding && cy < 8+padding && cy > 6-padding)
  {
    if (cx < MAX_Cells/2+26+padding && cx > MAX_Cells/2+25.75f-padding)
    {
      cx = MAX_Cells/2+26+padding;
      cxi = -cxi;
      cxi *= bounce;
    }
    else if (cx > MAX_Cells/2-26-padding && cx < MAX_Cells/2-25.75f+padding)
    {
      cx = MAX_Cells/2-26-padding;
      cxi = -cxi;
      cxi *= bounce;
    }


    if (cy < 8+padding && cy > 7.75f-padding)
    {
      cyi = -cyi;
      cyi *= bounce;
    }
    else if (cy > 6-padding && cy < 6.25f+padding)
    {
      cyi = -cyi;
      cyi *= bounce;
        }

    if (cz < MAX_Cells/2+26+padding && cz > MAX_Cells/2+25.75f-padding)
    {
      cz = MAX_Cells/2+26+padding;
      czi = -czi;
      czi *= bounce;
    }
    else if (cz > MAX_Cells/2-26-padding && cz < MAX_Cells/2-25.75f+padding)
    {
      cz = MAX_Cells/2-26-padding;
      czi = -czi;
      czi *= bounce;
    }

    Status = true;
  }

  return Status;
}


inline void Ricochet()
{
  int i;

  for (i = 0; i < MAX_PARTICLES; i++)
  {
    if (i == 0)
      particle[i][shots_fired].size = 2;
    else
      particle[i][shots_fired].size = GLdouble((rand()%25)+25)/50;

    particle[i][shots_fired].triangle_x1 = GLdouble((rand()%50)-25)/75.f;
    particle[i][shots_fired].triangle_x2 = GLdouble((rand()%50)-25)/75.f;
    particle[i][shots_fired].triangle_x3 = GLdouble((rand()%50)-25)/75.f;

    particle[i][shots_fired].triangle_y1 = GLdouble((rand()%50)-25)/75.f;
    particle[i][shots_fired].triangle_y2 = GLdouble((rand()%50)-25)/75.f;
    particle[i][shots_fired].triangle_y3 = GLdouble((rand()%50)-25)/75.f;

    particle[i][shots_fired].triangle_z1 = GLdouble((rand()%50)-25)/75.f;
    particle[i][shots_fired].triangle_z2 = GLdouble((rand()%50)-25)/75.f;
    particle[i][shots_fired].triangle_z3 = GLdouble((rand()%50)-25)/75.f;

    particle[i][shots_fired].triangle_rotate_x = rand()%360;
    particle[i][shots_fired].triangle_rotate_y = rand()%360;
    particle[i][shots_fired].triangle_rotate_z = rand()%360;

    particle[i][shots_fired].triangle_rotate_xi = GLdouble((rand()%50)-25)/5;
    particle[i][shots_fired].triangle_rotate_yi = GLdouble((rand()%50)-25)/5;
    particle[i][shots_fired].triangle_rotate_zi = GLdouble((rand()%50)-25)/5;

    particle[i][shots_fired].life = 1;
    particle[i][shots_fired].fade = GLdouble(rand()%100)/5000 + .02f;
    particle[i][shots_fired].x = fire_x;
    particle[i][shots_fired].y = fire_y;
    particle[i][shots_fired].z = fire_z;
    GLdouble angle = rand()%360;
    GLdouble angle2 = rand()%360;
    GLdouble speed = GLdouble(rand()%100) / 500;
    GLdouble Hyp = Hypot(angle,angle2);

    particle[i][shots_fired].xi = sin(Hypot(angle,Hyp))*speed + fire_xp;
    particle[i][shots_fired].yi = cos(Hypot(angle,Hyp))*speed + fire_yp;
    particle[i][shots_fired].zi = cos(Hypot(angle2,Hyp))*speed + fire_zp;
  }
}

inline int DrawGLScene(void)                                    // Here's Where We Do All The Drawing
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    // Clear The Screen And The Depth Buffer

  XP *= 0.8f;
  ZP *= 0.8f;
  xtrans2 += XP/10;
  ztrans2 += ZP/10;

  xtrans = -xtrans2;
  ztrans = -ztrans2;

  DetectCollision(xtrans2, ytrans2, ztrans2, XP, BlankNum, ZP, 2.5f, 0);

/*
  if(xtrans < -MAX_Cells+1.5f && XP >= 0) -MAX_Cells+1.5f, XP = 0;
  else if(xtrans > -1.5f && XP <= 0) xtrans=-1.5f, XP = 0;
  if(ztrans < -MAX_Cells+1.5f && ZP >= 0) ztrans=-MAX_Cells+1.5f, ZP = 0;
  else if(ztrans > -1.5f && ZP <= 0) ztrans=-1.5f, ZP = 0;

  if (xtrans < -MAX_Cells + 1.5f) xtrans = -MAX_Cells + 1.5f;
  else if (xtrans > -1.5f) xtrans = -1.5f;
  if (ztrans < -MAX_Cells + 1.5f) ztrans = -MAX_Cells + 1.5f;
  else if (ztrans > -1.5f) ztrans = -1.5f;
*/
/*
  if(XP >= 0 && xtrans > -MAX_Cells/2+24.f && xtrans < -MAX_Cells/2+26.5f && ztrans < -MAX_Cells/2+26 && ztrans > -MAX_Cells/2-26) xtrans = -MAX_Cells/2+26.5f, XP = 0;
  else if(XP <= 0 && xtrans < -MAX_Cells/2-24.f  && xtrans > -MAX_Cells/2-26.5f && ztrans < -MAX_Cells/2+26 && ztrans > -MAX_Cells/2-26) xtrans = -MAX_Cells/2-26.5f, XP = 0;

  if(ZP >= 0 && ztrans > -MAX_Cells/2+24.f && ztrans < -MAX_Cells/2+26.5f && xtrans < -MAX_Cells/2+26 && xtrans > -MAX_Cells/2-26) ztrans = -MAX_Cells/2+26.5f, ZP = 0;
  else if( ZP <= 0 && ztrans < -MAX_Cells/2-24.f  && ztrans > -MAX_Cells/2-26.5f && xtrans < -MAX_Cells/2+26 && xtrans > -MAX_Cells/2-26) ztrans = -MAX_Cells/2-26.5f, ZP = 0;
*/
  zprot*=.9f;
  _heading += zprot;
  heading = mouse_3d_x + _heading;

  yrot = heading;

  sceneroty = 360.0f - yrot;

  glLoadIdentity();
  m16_kicki += -m16_kick/50;
  m16_kick += m16_kicki/10;
  m16_kicki *= .8f;
  m16_kick *= .9f;
  if (m16_kick != 0)
  {
    glRotated(-m16_kick,1,0,0);
    glTranslatef(0,0,-m16_kick);
  }

  glRotated(mouse_3d_y,1.f,0,0);
  glRotated(sceneroty,0,1.f,0);

  glTranslated(xtrans,-10,ztrans);

  glCallList(ROOM);

  // Determine if you have fired a weapon
  if (isFire)
  {
    if (is_m16)
    {
      Sound1[shots_fired].Play ( 0 );
      m16_kicki = 1;
    }
    else
      Sound2[shots_fired].Play ( 0 );

    shots_fired++;
    if (shots_fired > MAX_SHOTS_FIRED-1) shots_fired = 0;
    isFireComplete = false;
    isFire = false;

    fire_x = -xtrans;
    fire_y = 10;
    fire_z = -ztrans;

    fire_xp = -(GLdouble)sin(heading*piover180) / 5;
    fire_yp = -(GLdouble)tan(mouse_3d_y*piover180) / 5;
    fire_zp = -(GLdouble)cos(heading*piover180) / 5;
  }
  int fire_counter = 0;

  // Begin calculations for the projectile
  while (!isFireComplete)
  {
    fire_counter++;
    fire_x += fire_xp;
    fire_y += fire_yp;
    fire_z += fire_zp;

    if (DetectCollision(fire_x,  fire_y,  fire_z, BlankNum, BlankNum, BlankNum, 0, 0))
    {
      isFireComplete = true;
      if (is_m16)
      {
        Ricochet();
        isRicochet = true;
      }
      else if (is_shock_rifle)
      {
        if (fire_z > MAX_Cells)
          fire_z = MAX_Cells;
        else if (fire_z < 0)
          fire_z = 0;
        if (fire_x > MAX_Cells)
          fire_x = MAX_Cells;
        else if (fire_x < 0)
          fire_x = 0;
        is_shock_rifle_blast = true;
      }
    }
    else if (fire_counter > 10000)
    {
      isFireComplete = true;
    }
  }

  if (shock_rifle_blast_delay > 0) shock_rifle_blast_delay--;

  // Determine if it was the Shock Rifle that was fired.
  if (is_shock_rifle_blast)
  {
    shock_rifle_blast_delay = 5;
    shock_rifle_blast_life -= .015f;
    shock_rifle_blast_life2 -= .1f;
    shock_rifle_blast_life3 += shock_rifle_blast_life3i;
    shock_rifle_blast_life3i -= .0001f*shock_rifle_blast_size;
    shock_rifle_blast_size += .5f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    GLdouble alpha;
    float r = .25f;
    float g = .4f;
    float b = 1;
    GLdouble light_size = 100;
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    alpha = shock_rifle_blast_life3 - (fire_y/light_size);
    if (alpha < 0) alpha = 0;
    else
    {
      glColor4d(r,g,b,alpha);
      glBegin(GL_QUADS);
      glTexCoord2d(1,1); glVertex3d(fire_x + alpha*light_size, 0.1f, fire_z + alpha*light_size);
      glTexCoord2d(1,0); glVertex3d(fire_x + alpha*light_size, 0.1f, fire_z - alpha*light_size);
      glTexCoord2d(0,0); glVertex3d(fire_x - alpha*light_size, 0.1f, fire_z - alpha*light_size);
      glTexCoord2d(0,1); glVertex3d(fire_x - alpha*light_size, 0.1f, fire_z + alpha*light_size);
      glEnd();
    }

    alpha = shock_rifle_blast_life3 - ((MAX_Cells/4-fire_y)/light_size);
    if (alpha < 0) alpha = 0;
    else
    {
      glColor4d(r,g,b,alpha);
      glBegin(GL_QUADS);
      glTexCoord2d(1,1); glVertex3d(fire_x + alpha*light_size, MAX_Cells/4-.1f, fire_z + alpha*light_size);
      glTexCoord2d(1,0); glVertex3d(fire_x + alpha*light_size, MAX_Cells/4-.1f, fire_z - alpha*light_size);
      glTexCoord2d(0,0); glVertex3d(fire_x - alpha*light_size, MAX_Cells/4-.1f, fire_z - alpha*light_size);
      glTexCoord2d(0,1); glVertex3d(fire_x - alpha*light_size, MAX_Cells/4-.1f, fire_z + alpha*light_size);
      glEnd();
    }

    alpha = shock_rifle_blast_life3 - (fire_z/light_size);
    if (alpha < 0) alpha = 0;
    else
    {
      glColor4d(r,g,b,alpha);
      glBegin(GL_QUADS);
      glTexCoord2d(1,1); glVertex3d(fire_x + alpha*light_size, fire_y + alpha*light_size, .1f);
      glTexCoord2d(1,0); glVertex3d(fire_x + alpha*light_size, fire_y - alpha*light_size, .1f);
      glTexCoord2d(0,0); glVertex3d(fire_x - alpha*light_size, fire_y - alpha*light_size, .1f);
      glTexCoord2d(0,1); glVertex3d(fire_x - alpha*light_size, fire_y + alpha*light_size, .1f);
      glEnd();
    }

    alpha = shock_rifle_blast_life3 - ((MAX_Cells-fire_z)/light_size);
    if (alpha < 0) alpha = 0;
    else
    {
        glColor4d(r,g,b,alpha);
        glBegin(GL_QUADS);
        glTexCoord2d(1,1); glVertex3d(fire_x + alpha*light_size, fire_y + alpha*light_size, MAX_Cells-.1f);
        glTexCoord2d(1,0); glVertex3d(fire_x + alpha*light_size, fire_y - alpha*light_size, MAX_Cells-.1f);
        glTexCoord2d(0,0); glVertex3d(fire_x - alpha*light_size, fire_y - alpha*light_size, MAX_Cells-.1f);
        glTexCoord2d(0,1); glVertex3d(fire_x - alpha*light_size, fire_y + alpha*light_size, MAX_Cells-.1f);
        glEnd();
    }

    alpha = shock_rifle_blast_life3 - (fire_x/light_size);
    if (alpha < 0) alpha = 0;
    else
    {
      glColor4d(r,g,b,alpha);
      glBegin(GL_QUADS);
      glTexCoord2d(1,1); glVertex3d(.1f, fire_y + alpha*light_size, fire_z + alpha*light_size);
      glTexCoord2d(1,0); glVertex3d(.1f, fire_y - alpha*light_size, fire_z + alpha*light_size);
      glTexCoord2d(0,0); glVertex3d(.1f, fire_y - alpha*light_size, fire_z - alpha*light_size);
      glTexCoord2d(0,1); glVertex3d(.1f, fire_y + alpha*light_size, fire_z - alpha*light_size);
      glEnd();
    }

    alpha = shock_rifle_blast_life3 - ((MAX_Cells-fire_x)/light_size);
    if (alpha < 0) alpha = 0;
    else
    {
      glColor4d(r,g,b,alpha);
      glBegin(GL_QUADS);
      glTexCoord2d(1,1); glVertex3d(MAX_Cells-.1f, fire_y + alpha*light_size, fire_z + alpha*light_size);
      glTexCoord2d(1,0); glVertex3d(MAX_Cells-.1f, fire_y - alpha*light_size, fire_z + alpha*light_size);
      glTexCoord2d(0,0); glVertex3d(MAX_Cells-.1f, fire_y - alpha*light_size, fire_z - alpha*light_size);
      glTexCoord2d(0,1); glVertex3d(MAX_Cells-.1f, fire_y + alpha*light_size, fire_z - alpha*light_size);
      glEnd();
    }

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glColor4d(1,1,1,shock_rifle_blast_life2);
    glBegin(GL_QUADS);
    glTexCoord2d(0,1); glVertex3d(-1.5f,-2,0);
    glTexCoord2d(1,1); glVertex3d(1.5f,-2,0);
    glTexCoord2d(1,0); glVertex3d(1.5f,0,-500);
    glTexCoord2d(0,0); glVertex3d(-1.5f,0,-500);
    glEnd();

    glLoadIdentity();
    glRotated(mouse_3d_y,1.f,0,0);
    glRotated(sceneroty,0,1.f,0);
    glTranslated(xtrans,-10,ztrans);
    glTranslated(fire_x,fire_y,fire_z);
    glRotated(-sceneroty,0,1.f,0);
    glRotated(-mouse_3d_y,1.f,0,0);

    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glColor4d(1,1,1,shock_rifle_blast_life);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1); glVertex3d( shock_rifle_blast_size, shock_rifle_blast_size,0);
    glTexCoord2d(1,0); glVertex3d( shock_rifle_blast_size,-shock_rifle_blast_size,0);
    glTexCoord2d(0,0); glVertex3d(-shock_rifle_blast_size,-shock_rifle_blast_size,0);
    glTexCoord2d(0,1); glVertex3d(-shock_rifle_blast_size, shock_rifle_blast_size,0);
    glEnd();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    if (shock_rifle_blast_life <= 0)
    {
      is_shock_rifle_blast = false;
      shock_rifle_blast_life = 1;
      shock_rifle_blast_life2 = 1;
      shock_rifle_blast_life3 = .5f;
      shock_rifle_blast_life3i = .015f;
      shock_rifle_blast_size = .1f;
    }
  }

  // Determine if it was the M16 that was fired.
  if (isRicochet)
  {
    int i;
    int j;
    bool isRicochetComplete = true;
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    for (j = 0; j < MAX_SHOTS_FIRED; j++)
    {
      for (i = 8; i < MAX_PARTICLES; i++)
      {
        if (particle[i][j].life > 0)
        {
          isRicochetComplete = false;

          particle[i][j].x += particle[i][j].xi;
          particle[i][j].y += particle[i][j].yi;
          particle[i][j].z += particle[i][j].zi;

          DetectCollision(particle[i][j].x,  particle[i][j].y,  particle[i][j].z,
                          particle[i][j].xi, particle[i][j].yi, particle[i][j].zi, 0, .75f);

          particle[i][j].yi -= .01f;  // cheap simulated gravity
          particle[i][j].xi *= .99;
          particle[i][j].yi *= .99;
          particle[i][j].zi *= .99;
          particle[i][j].life -= particle[i][j].fade;

          glLoadIdentity();
          glRotated(mouse_3d_y,1.f,0,0);
          glRotated(sceneroty,0,1.f,0);
          glTranslated(xtrans,-10,ztrans);
          glTranslated(particle[i][j].x,particle[i][j].y,particle[i][j].z);
          glRotated(-sceneroty,0,1.f,0);
          glRotated(-mouse_3d_y,1.f,0,0);

          particle[i][j].triangle_rotate_x += particle[i][j].triangle_rotate_xi;
          particle[i][j].triangle_rotate_y += particle[i][j].triangle_rotate_yi;
          particle[i][j].triangle_rotate_z += particle[i][j].triangle_rotate_zi;
          particle[i][j].triangle_rotate_xi -= particle[i][j].fade;
          particle[i][j].triangle_rotate_yi -= particle[i][j].fade;
          particle[i][j].triangle_rotate_zi -= particle[i][j].fade;
          glRotated(particle[i][j].triangle_rotate_x,1,0,0);
          glRotated(particle[i][j].triangle_rotate_y,0,1,0);
          glRotated(particle[i][j].triangle_rotate_z,0,0,1);

          glColor4d(0,0,0,particle[i][j].life);

          glBegin(GL_TRIANGLES);
          glVertex3d(particle[i][j].triangle_x1, particle[i][j].triangle_y1, particle[i][j].triangle_z1);
          glVertex3d(particle[i][j].triangle_x2, particle[i][j].triangle_y2, particle[i][j].triangle_z2);
          glVertex3d(particle[i][j].triangle_x3, particle[i][j].triangle_y3, particle[i][j].triangle_z3);
          glEnd();
        }
      }
    }
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    glBindTexture(GL_TEXTURE_2D, texture[16]);

    // Display particles of the ricochet from the M16
    for (j = 0; j < MAX_SHOTS_FIRED; j++)
    {
      for (i = 0; i < 8; i++)
      {
        if (particle[i][j].life > 0)
        {
          isRicochetComplete = false;

          particle[i][j].x += particle[i][j].xi/2;
          particle[i][j].y += particle[i][j].yi/2;
          particle[i][j].z += particle[i][j].zi/2;

          particle[i][j].yi += .01f;

          particle[i][j].xi *= .9f;
          particle[i][j].yi *= .9f;
          particle[i][j].zi *= .9f;

          particle[i][j].life -= particle[i][j].fade/2;

          glLoadIdentity();
          glRotated(mouse_3d_y,1.f,0,0);
          glRotated(sceneroty,0,1.f,0);
          glTranslated(xtrans,-10,ztrans);
          glTranslated(particle[i][j].x,particle[i][j].y,particle[i][j].z);
          glRotated(-sceneroty,0,1.f,0);
          glRotated(-mouse_3d_y,1.f,0,0);

          glColor4d(.18f,.17f,.15f,particle[i][j].life);
          glBegin(GL_QUADS);
          glTexCoord2d(1,1); glVertex3d( particle[i][j].size, particle[i][j].size,0);
          glTexCoord2d(1,0); glVertex3d( particle[i][j].size,-particle[i][j].size,0);
          glTexCoord2d(0,0); glVertex3d(-particle[i][j].size,-particle[i][j].size,0);
          glTexCoord2d(0,1); glVertex3d(-particle[i][j].size, particle[i][j].size,0);
          glEnd();
        }
      }
    }
    glEnable(GL_DEPTH_TEST);

    if (isRicochetComplete)
      isRicochet = false;
  }

  // Draw HUD
  glDisable(GL_DEPTH_TEST);

  glLoadIdentity();
  glTranslated(-765,-567.0f,-1000.0f);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glColor4d(.4f,.2f,0,.5f);
  glVertex2f(-10,-10);
  glVertex2f(-10,71+10);
  glColor4d(1,1,1,0);
  glVertex2f(512+10,71+10);
  glVertex2f(512+10,-10);
  glEnd();
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);

  glBindTexture(GL_TEXTURE_2D,texture[11]);

  if (is_m16)
    glColor4d(1,.5f,0,1);
  else
    glColor4d(1,1,1,.15f);
  glBegin(GL_QUADS);
  glTexCoord2d(1,1); glVertex2f(256,71);
  glTexCoord2d(1,0); glVertex2f(256,0);
  glTexCoord2d(0,0); glVertex2f(0,0);
  glTexCoord2d(0,1); glVertex2f(0,71);
  glEnd();

  glTranslated(256,0,0);
  glBindTexture(GL_TEXTURE_2D,texture[12]);

  if (is_shock_rifle)
    glColor4d(1,.5f,0,1);
  else
    glColor4d(1,1,1,.15f);
  glBegin(GL_QUADS);
  glTexCoord2d(1,1); glVertex2f(256,71);
  glTexCoord2d(1,0); glVertex2f(256,0);
  glTexCoord2d(0,0); glVertex2f(0,0);
  glTexCoord2d(0,1); glVertex2f(0,71);
  glEnd();

  glEnable(GL_DEPTH_TEST);

  // Crosshair
  glDisable(GL_DEPTH_TEST);
  glLoadIdentity();
  glTranslated(0.0f,0.0f,-35.0f);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE);

  glColor4f(1,1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[15]);
  glBegin(GL_QUADS);
  glTexCoord2d(1,1); glVertex2f(1,1);
  glTexCoord2d(1,0); glVertex2f(1,-1);
  glTexCoord2d(0,0); glVertex2f(-1,-1);
  glTexCoord2d(0,1); glVertex2f(-1,1);
  glEnd();

  glEnable(GL_DEPTH_TEST);


//  ESTABLISH A FPS FRAMERATE COUNTER
  if (isFPS)
  {
    glDisable(GL_DEPTH_TEST);
    frames++;
    if (frames%10 == 0)
    {
      Time2 = TimerGetTime()/1000;
      DiffTime = ABS(Time2-Time1);
      Time1 = TimerGetTime()/1000;
      if (DiffTime != 0)
          FPS = 10/(DiffTime);
    }

      glBindTexture(GL_TEXTURE_2D, textures[0].texID);        // Select Our Font Texture
    glColor4d(0,1,0,1);
      glPrint(-350,250,1,"FPS: %4.0f", FPS);
      //glPrint(-350,230,1,"Elapsed Time: %4.2f", Time1-StartTime);

      glEnable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
  }
  else
  {
    Time2 = TimerGetTime()/1000;
    DiffTime = ABS(Time2-Time1);
    while (DiffTime < .015f) // .015 = 66 frames per second
    {
      Sleep(1);
      Time2 = TimerGetTime()/1000;
      DiffTime = ABS(Time2-Time1);
      }
      Time1 = TimerGetTime()/1000;
  }

  return TRUE;                                        // Keep Going
}

void KillGLWindow(void)                                // Properly Kill The Window
{
    for (int i = 0; i < MAX_SHOTS_FIRED; i++)
    {
    Sound1[i].ReleaseBuffer();
    Sound2[i].ReleaseBuffer();
    }
    SoundManager.Uninitialize();

    KillFont();
    if (hRC)                                            // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL,NULL))                    // Are We Able To Release The DC And RC Contexts?
        {
            MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC))                        // Are We Able To Delete The RC?
        {
            MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        hRC=NULL;                                        // Set RC To NULL
    }

    if (hDC && !ReleaseDC(hWnd,hDC))                    // Are We Able To Release The DC
    {
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDC=NULL;                                        // Set DC To NULL
    }

    if (hWnd && !DestroyWindow(hWnd))                    // Are We Able To Destroy The Window?
    {
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;                                        // Set hWnd To NULL
    }

    if (fullscreen)                                        // Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL,0);                    // If So Switch Back To The Desktop
        ShowCursor(TRUE);                                // Show Mouse Pointer
    }
}

/*    This Code Creates Our OpenGL Window.  Parameters Are:                    *
 *    title            - Title To Appear At The Top Of The Window                *
 *    width            - Width Of The GL Window Or Fullscreen Mode                *
 *    height            - Height Of The GL Window Or Fullscreen Mode            *
 *    bits            - Number Of Bits To Use For Color (8/16/24/32)            *
 *    fullscreenflag    - Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)    */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint        PixelFormat;            // Holds The Results After Searching For A Match
    HINSTANCE    hInstance;                // Holds The Instance Of The Application
    WNDCLASS    wc;                        // Windows Class Structure
    DWORD        dwExStyle;                // Window Extended Style
    DWORD        dwStyle;                // Window Style


    fullscreen=fullscreenflag;            // Set The Global Fullscreen Flag

    hInstance            = GetModuleHandle(NULL);                // Grab An Instance For Our Window
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;    // Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc        = (WNDPROC) WndProc;                    // WndProc Handles Messages
    wc.cbClsExtra        = 0;                                    // No Extra Window Data
    wc.cbWndExtra        = 0;                                    // No Extra Window Data
    wc.hInstance        = hInstance;                            // Set The Instance
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);            // Load The Default Icon
    wc.hCursor            = LoadCursor(NULL, IDC_ARROW);            // Load The Arrow Pointer
    wc.hbrBackground    = NULL;                                    // No Background Required For GL
    wc.lpszMenuName        = NULL;                                    // We Don't Want A Menu
    wc.lpszClassName    = "OpenGL";                                // Set The Class Name

    ChangeDisplaySettings(NULL,0);                    // If So Switch Back To The Desktop

    if (!RegisterClass(&wc))                                    // Attempt To Register The Window Class
    {
        MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                            // Return FALSE
    }

    if (fullscreen)                                                // Attempt Fullscreen Mode?
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
            if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
            {
                fullscreen=FALSE;        // Windowed Mode Selected.  Fullscreen = FALSE
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
                return FALSE;                                    // Return FALSE
            }
        }
    }

    if (fullscreen)                                                // Are We Still In Fullscreen Mode?
    {
        dwExStyle=WS_EX_APPWINDOW;                                // Window Extended Style
        dwStyle=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;    // Windows Style
        ShowCursor(FALSE);                                        // Hide Mouse Pointer
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                        // Window Extended Style
        dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;    // Windows Style
    }

    // Create The Window
    if (!(hWnd=CreateWindowEx(    dwExStyle,            // Extended Style For The Window
                                "OpenGL",            // Class Name
                                title,                // Window Title
                                dwStyle,            // Window Style
                                0, 0,                // Window Position
                                width, height,        // Selected Width And Height
                                NULL,                // No Parent Window
                                NULL,                // No Menu
                                hInstance,            // Instance
                                NULL)))                // Dont Pass Anything To WM_CREATE
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    static    PIXELFORMATDESCRIPTOR pfd=                // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),                // Size Of This Pixel Format Descriptor
        1,                                            // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                            // Must Support Double Buffering
        PFD_TYPE_RGBA,                                // Request An RGBA Format
        bits,                                        // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                            // Color Bits Ignored
        0,                                            // No Alpha Buffer
        0,                                            // Shift Bit Ignored
        0,                                            // No Accumulation Buffer
        0, 0, 0, 0,                                    // Accumulation Bits Ignored
        16,                                            // 16Bit Z-Buffer (Depth Buffer)
        0,                                            // No Stencil Buffer
        0,                                            // No Auxiliary Buffer
        PFD_MAIN_PLANE,                                // Main Drawing Layer
        0,                                            // Reserved
        0, 0, 0                                        // Layer Masks Ignored
    };

    if (!(hDC=GetDC(hWnd)))                            // Did We Get A Device Context?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))    // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))        // Are We Able To Set The Pixel Format?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    if (!(hRC=wglCreateContext(hDC)))                // Are We Able To Get A Rendering Context?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    if(!wglMakeCurrent(hDC,hRC))                    // Try To Activate The Rendering Context
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    ShowWindow(hWnd,SW_SHOW);                        // Show The Window
    SetForegroundWindow(hWnd);                        // Slightly Higher Priority
    SetFocus(hWnd);                                    // Sets Keyboard Focus To The Window
    ReSizeGLScene(width, height);                    // Set Up Our Perspective GL Screen

    if (!InitGL())                                    // Initialize Our Newly Created GL Window
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;                                // Return FALSE
    }

    return TRUE;                                    // Success
}

LRESULT CALLBACK WndProc(    HWND    hWnd,            // Handle For This Window
                            UINT    uMsg,            // Message For This Window
                            WPARAM    wParam,            // Additional Message Information
                            LPARAM    lParam)            // Additional Message Information
{
  if (uMsg == WM_MOUSEMOVE)
  {
    if (LOWORD(lParam) > 370) SetCursorPos(10,(int)mouse_y), mouse_wrap_x--;
    else if (LOWORD(lParam) < 10) SetCursorPos(370,(int)mouse_y), mouse_wrap_x++;

    if (HIWORD(lParam) > 370) SetCursorPos((int)mouse_x,10), mouse_wrap_x--;
    else if (HIWORD(lParam) < 10) SetCursorPos((int)mouse_x,370), mouse_wrap_x++;

    if (ABS(mouse_x-LOWORD(lParam)) > 300)
    {
      if (mouse_x > 360/2)
        mouse_3d_x += ((mouse_x-360)-LOWORD(lParam))/sensitivity;
      else if (LOWORD(lParam) > 360/2)
        mouse_3d_x += (mouse_x-(LOWORD(lParam)-360))/sensitivity;
    }
    else
    {
      mouse_3d_x += (mouse_x-LOWORD(lParam))/sensitivity;
    }

    if (mouse_3d_x > 360) mouse_3d_x = 0;
    else if (mouse_3d_x < 0) mouse_3d_x = 360;

    if (invert_mouse)
    {
      if (ABS(mouse_y-HIWORD(lParam)) > 300)
      {
        if (mouse_y > 360/2)
          mouse_3d_y -= (HIWORD(lParam)-(mouse_y-360))/sensitivity;
        else if (LOWORD(lParam) > 360/2)
          mouse_3d_y -= ((HIWORD(lParam)-360)-mouse_y)/sensitivity;
      }
      else
      {
        mouse_3d_y -= (HIWORD(lParam)-mouse_y)/sensitivity;
      }
    }
    else
    {
      if (ABS(mouse_y-HIWORD(lParam)) > 300)
      {
        if (mouse_y > 360/2)
          mouse_3d_y -= ((mouse_y-360)-HIWORD(lParam))/sensitivity;
        else if (LOWORD(lParam) > 360/2)
          mouse_3d_y -= (mouse_y-(HIWORD(lParam)-360))/sensitivity;
      }
      else
      {
        mouse_3d_y -= (mouse_y-HIWORD(lParam))/sensitivity;
      }
    }

    if (mouse_3d_x > 360) mouse_3d_x = 0;
    else if (mouse_3d_x < 0) mouse_3d_x = 360;

    if (mouse_3d_y > 85) mouse_3d_y = 85;
    else if (mouse_3d_y < -85) mouse_3d_y = -85;

    mouse_x = LOWORD(lParam);
    mouse_y = HIWORD(lParam);
  }

  else if (uMsg == WM_LBUTTONDOWN)
  {
    if (!(is_shock_rifle && shock_rifle_blast_delay > 0))
      isFire = true;
  }
  else if (uMsg == WM_ACTIVATE)                            // Watch For Window Activate Message
  {
    if (!HIWORD(wParam))                    // Check Minimization State
    {
      active=TRUE;                        // Program Is Active
    }
    else
    {
      active=FALSE;                        // Program Is No Longer Active
    }

  }

  else if (uMsg == WM_SYSCOMMAND)                            // Intercept System Commands
  {
    switch (wParam)                            // Check System Calls
    {
      case SC_SCREENSAVE:                    // Screensaver Trying To Start?
      case SC_MONITORPOWER:                // Monitor Trying To Enter Powersave?

      return 0;                            // Prevent From Happening
    }
  }

  else if (uMsg == WM_CLOSE)                                // Did We Receive A Close Message?
  {
    PostQuitMessage(0);                        // Send A Quit Message
  }

  else if (uMsg == WM_KEYDOWN)                            // Is A Key Being Held Down?
  {
    keys[wParam] = TRUE;                    // If So, Mark It As TRUE
  }

  else if (uMsg == WM_KEYUP)                                // Has A Key Been Released?
  {
    keys[wParam] = FALSE;                    // If So, Mark It As FALSE
  }

  else if (uMsg == WM_SIZE)                                // Resize The OpenGL Window
  {
    ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
  }


  // Pass All Unhandled Messages To DefWindowProc
  return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int is_Fired = 0;

int WINAPI WinMain(    HINSTANCE    hInstance,            // Instance
                    HINSTANCE    hPrevInstance,        // Previous Instance
                    LPSTR        lpCmdLine,            // Command Line Parameters
                    int            nCmdShow)            // Window Show State
{

  MSG        msg;                                    // Windows Message Structure

  // Ask The User Which Screen Mode They Prefer

  fullscreen=true;

  if (!CreateGLWindow("Steve's First Person Shooter",800,600,32,fullscreen))
  {
    return 0;
  }

  while(!done)                                    // Loop That Runs While done=FALSE
  {
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))    // Is There A Message Waiting?
    {
      if (msg.message==WM_QUIT)                // Have We Received A Quit Message?
      {
        done=TRUE;                            // If So done=TRUE
      }
      else                                    // If Not, Deal With Window Messages
      {
        TranslateMessage(&msg);                // Translate The Message
        DispatchMessage(&msg);                // Dispatch The Message
      }
    }
    else                                        // If There Are No Messages
    {
      // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
      if ((active && !DrawGLScene()) || keys[VK_ESCAPE])    // Active?  Was There A Quit Received?
      {
                done=TRUE;                            // ESC or DrawGLScene Signalled A Quit
      }
      else                                    // Not Time To Quit, Update Screen
      {
        SwapBuffers(hDC);                    // Swap Buffers (Double Buffering)

        is_Fired--;
        if (is_Fired < 0) is_Fired = 0;

        if (keys[VK_UP])  // Move forwards
        {
          XP -= (GLdouble)sin(heading*piover180) * 5.0f;
          ZP -= (GLdouble)cos(heading*piover180) * 5.0f;
        }
        else if (keys['W'])  // Move forwards
        {
          XP -= (GLdouble)sin(heading*piover180) * 5.0f;
          ZP -= (GLdouble)cos(heading*piover180) * 5.0f;
        }

        if (keys[VK_DOWN]) // Move backwards
        {
          XP += (GLdouble)sin(heading*piover180) * 5.0f;
          ZP += (GLdouble)cos(heading*piover180) * 5.0f;
        }
        else if (keys['S']) // Move backwards
        {
          XP += (GLdouble)sin(heading*piover180) * 5.0f;
          ZP += (GLdouble)cos(heading*piover180) * 5.0f;
        }

        if (keys['A'])  // strafe left
        {
          XP += (GLdouble)sin((heading-90)*piover180) * 5.0f;
          ZP += (GLdouble)cos((heading-90)*piover180) * 5.0f;
        }

        if (keys['D']) // strafe right
        {
          XP += (GLdouble)sin((heading+90)*piover180) * 5.0f;
          ZP += (GLdouble)cos((heading+90)*piover180) * 5.0f;
        }

        if (keys['1'])  // Enable the M16
        {
          is_m16 = true;    // 1
          is_shock_rifle = false; // 2
        }

        if (keys['2'])  // Enable the Shock Rifle
        {
          is_m16 = false; // 1
          is_shock_rifle = true; // 2
        }

        if (keys[VK_SPACE])
        {
          if (is_m16 && is_Fired == 0)
          {
            is_Fired = 7;  // 1 shot every 7 frames.
            isFire = true;
          }
          else if ((is_shock_rifle && shock_rifle_blast_delay <= 0))
          {
            isFire = true;
          }
        }

        if (keys[VK_LEFT]) // Turn left
        {
          zprot += .5f;
        }
        else if (keys[VK_RIGHT]) // Turn right
        {
          zprot -= .5f;
        }

        if (keys['F'] && !fp) // Toggle Frame Per Second Counter
        {
          isFPS = !isFPS;
          fp = true;
        }
        else if (!keys['F'])
        {
          fp = false;
        }

        if (keys['I'] && !ip) // Invert y-axis for the mouse
        {
          invert_mouse = !invert_mouse;
          ip = true;
        }
        else if (!keys['I'])
        {
          ip = false;
        }
      }
    }
  }

  // Shutdown
  KillGLWindow();                                    // Kill The Window
  return (msg.wParam);                        // Exit The Program
}
