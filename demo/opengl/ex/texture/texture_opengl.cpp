#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <GL/gl.h>
#include <GL/glu.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "uglut.h"

#define MAX_VERTICES 2000 // Max number of vertices (for each object)
#define MAX_POLYGONS 2000 // Max number of polygons (for each object)

int num_texture=-1; //Counter to keep track of the last loaded texture

int LoadBitmap(char *filename)
{
    int i, j=0; //Index variables
    FILE *l_file; //File pointer
    unsigned char *l_texture; //The pointer to the memory zone in which we will load the texture

    // windows.h gives us these types to work with the Bitmap files
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++; // The counter of the current texture is increased

    if( (l_file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading

    fread(&fileheader, sizeof(fileheader), 1, l_file); // Read the fileheader

    fseek(l_file, sizeof(fileheader), SEEK_SET); // Jump the fileheader
    fread(&infoheader, sizeof(infoheader), 1, l_file); // and read the infoheader

    // Now we need to allocate the memory for our image (width * height * color deep)
    l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    // And fill it with zeros
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

    // At this point we can read every pixel of the image
    for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {
        // We load an RGB value from the file
        fread(&rgb, sizeof(rgb), 1, l_file);

        // And store it
        l_texture[j+0] = rgb.rgbtRed; // Red component
        l_texture[j+1] = rgb.rgbtGreen; // Green component
        l_texture[j+2] = rgb.rgbtBlue; // Blue component
        l_texture[j+3] = 255; // Alpha value
        j += 4; // Go to the next position
    }

    fclose(l_file); // Closes the file stream

    glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture); // Free the memory we used to load the texture

    return (num_texture); // Returns the current texture OpenGL ID
}

// Our vertex type
typedef struct{
    float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    float u,v;
}mapcoord_type;

// The object type
typedef struct {
    vertex_type vertex[MAX_VERTICES];
    polygon_type polygon[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
    int id_texture;
} obj_type, *obj_type_ptr;

// Absolute rotation values (0-359 degrees) and rotation increments for each frame
double rotation_x=0, rotation_x_increment=0.1;
double rotation_y=0, rotation_y_increment=0.05;
double rotation_z=0, rotation_z_increment=0.03;

// Flag for rendering as lines or filled polygons
int filling=1; //0=OFF 1=ON

// And, finally our first object!
obj_type cube =
{
    {
        -10, -10, 10,   // vertex v0
        10,  -10, 10,   // vertex v1
        10,  -10, -10,  // vertex v2
        -10, -10, -10,  // vertex v3
        -10, 10,  10,   // vertex v4
        10,  10,  10,   // vertex v5
        10,  10,  -10,  // vertex v6
        -10, 10,  -10   // vertex v7
    },
    {
        0, 1, 4,  // polygon v0,v1,v4
        1, 5, 4,  // polygon v1,v5,v4
        1, 2, 5,  // polygon v1,v2,v5
        2, 6, 5,  // polygon v2,v6,v5
        2, 3, 6,  // polygon v2,v3,v6
        3, 7, 6,  // polygon v3,v7,v6
        3, 0, 7,  // polygon v3,v0,v7
        0, 4, 7,  // polygon v0,v4,v7
        4, 5, 7,  // polygon v4,v5,v7
        5, 6, 7,  // polygon v5,v6,v7
        3, 2, 0,  // polygon v3,v2,v0
        2, 1, 0   // polygon v2,v1,v0
    },
    {
        0.0, 0.0,  // mapping coordinates for vertex v0
        1.0, 0.0,  // mapping coordinates for vertex v1
        1.0, 0.0,  // mapping coordinates for vertex v2
        0.0, 0.0,  // mapping coordinates for vertex v3
        0.0, 1.0,  // mapping coordinates for vertex v4
        1.0, 1.0,  // mapping coordinates for vertex v5
        1.0, 1.0,  // mapping coordinates for vertex v6
        0.0, 1.0   // mapping coordinates for vertex v7
    },
    0,
};


//
using huys::UGLDialog;

class MyGLDialog : public UGLDialog
{
public:
    MyGLDialog(HINSTANCE hInst, UINT nID)
    : UGLDialog(hInst, nID)
    {}

    BOOL initGL()
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        int w = rc.right-rc.left;
        int h = rc.bottom-rc.top;
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
        glViewport(0,0,w,h); // Viewport transformation

        glMatrixMode(GL_PROJECTION); // Projection transformation
        glLoadIdentity(); // We initialize the projection matrix as identity
        gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,1.0f,1000.0f);

        glClearColor(0.0, 0.0, 0.2, 0.0); // This clear the background color to dark blue
        glShadeModel(GL_SMOOTH); // Type of shading for the polygons

        // Viewport transformation
        glViewport(0,0,w,h);

        // Projection transformation
        glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
        glLoadIdentity(); // We initialize the projection matrix as identity
        gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,1.0f,1000.0f); // We define the "viewing volume"

        glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

        glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

        cube.id_texture=LoadBitmap("texture.bmp"); // The Function LoadBitmap() return the current texture ID

        // If the last function returns -1 it means the file was not found so we exit from the program
        if (cube.id_texture==-1)
        {
            MessageBox(NULL,"Image file: texture1.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
            exit (0);
        }

        return TRUE;
    }

    virtual BOOL animate()
    {
        int l_index;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
        glMatrixMode(GL_MODELVIEW); // Modeling transformation
        glLoadIdentity(); // Initialize the model matrix as identity

        glTranslatef(0.0,0.0,-50); // We move the object 50 points forward (the model matrix is multiplied by the translation matrix)

        rotation_x = rotation_x + rotation_x_increment;
        rotation_y = rotation_y + rotation_y_increment;
        rotation_z = rotation_z + rotation_z_increment;

        if (rotation_x > 359) rotation_x = 0;
        if (rotation_y > 359) rotation_y = 0;
        if (rotation_z > 359) rotation_z = 0;

        glRotatef(rotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
        glRotatef(rotation_y,0.0,1.0,0.0);
        glRotatef(rotation_z,0.0,0.0,1.0);

        glBindTexture(GL_TEXTURE_2D, cube.id_texture); // We set the active texture

        glBegin(GL_TRIANGLES); // GlBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
        for (l_index=0;l_index<12;l_index++)
        {
            //----------------- FIRST VERTEX -----------------
            // Texture coordinates of the first vertex
            glTexCoord2f( cube.mapcoord[ cube.polygon[l_index].a ].u,
            cube.mapcoord[ cube.polygon[l_index].a ].v);
            // Coordinates of the first vertex
            glVertex3f( cube.vertex[ cube.polygon[l_index].a ].x,
            cube.vertex[ cube.polygon[l_index].a ].y,
            cube.vertex[ cube.polygon[l_index].a ].z); //Vertex definition

            //----------------- SECOND VERTEX -----------------
            // Texture coordinates of the second vertex
            glTexCoord2f( cube.mapcoord[ cube.polygon[l_index].b ].u,
            cube.mapcoord[ cube.polygon[l_index].b ].v);
            // Coordinates of the second vertex
            glVertex3f( cube.vertex[ cube.polygon[l_index].b ].x,
            cube.vertex[ cube.polygon[l_index].b ].y,
            cube.vertex[ cube.polygon[l_index].b ].z);

            //----------------- THIRD VERTEX -----------------
            // Texture coordinates of the third vertex
            glTexCoord2f( cube.mapcoord[ cube.polygon[l_index].c ].u,
            cube.mapcoord[ cube.polygon[l_index].c ].v);
            // Coordinates of the Third vertex
            glVertex3f( cube.vertex[ cube.polygon[l_index].c ].x,
            cube.vertex[ cube.polygon[l_index].c ].y,
            cube.vertex[ cube.polygon[l_index].c ].z);
        }
        glEnd();

        glFlush(); // This force the execution of OpenGL commands
        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UGLDialog::onCancel();
        case 'w':
            {
                rotation_x_increment += 0.05;
                return FALSE;
            }
        case 's':
            {
                rotation_x_increment -= 0.05;
                return FALSE;
            }
        case 'a':
            {
                rotation_y_increment += 0.05;
                return FALSE;
            }
        case 'd':
            {
                rotation_y_increment -= 0.05;
                return FALSE;
            }
        case 'r':
            if (filling==0)
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
                filling=1;
            }
            else
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
                filling=0;
            }
            return FALSE;
        default:
            return UGLDialog::onChar(wParam, lParam);
        }
    }
};

UDLGAPP_T(MyGLDialog, IDD_TEST);

