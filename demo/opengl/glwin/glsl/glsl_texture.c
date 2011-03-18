#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h> // Needed on some systems

/* Global function pointers for extensions we need to load in Windows */
/*
PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB        = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB  = NULL;
PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB         = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB     = NULL;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB   = NULL;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB         = NULL;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB        = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB         = NULL;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB           = NULL;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB          = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB   = NULL;
PFNGLUNIFORM4FARBPROC            glUniform4fARB            = NULL;
PFNGLUNIFORM1FARBPROC            glUniform1fARB            = NULL;
PFNGLUNIFORM1IARBPROC            glUniform1iARB            = NULL;
*/

/*
 * printError - Signal an error.
 */
void printError(const char *errtype, const char *errmsg)
{
  fprintf(stderr, "%s: %s\n", errtype, errmsg);
}


/*
 * filelength - Determine the number of bytes in a file.
 * This is a lazy hack to avoid calling stat(), but it works.
 */
int filelength(const char *filename)
{
  FILE *ifp;
  int length = 0;
  
  ifp = fopen(filename, "r");
  fseek(ifp, 0, SEEK_END);
  length = (int)ftell(ifp);
  fclose(ifp);
  return length;
}


/*
 * loadExtensions - Load the required OpenGL extensions.
 */
void loadExtensions()
{
  // These extension strings indicate that the OpenGL Shading Language,
  // GLSL shader objects and floating-point textures are supported.
  /*
  if(!glfwExtensionSupported("GL_ARB_shading_language_100"))
    {
      printError("GL init error", "GL_ARB_shading_language_100 extension was not found");
      return;
    }
  if(!glfwExtensionSupported("GL_ARB_shader_objects"))
    {
      printError("GL init error", "GL_ARB_shader_objects extension was not found");
      return;
    }

  // else
  glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)glfwGetProcAddress("glActiveTextureARB");
  glCreateProgramObjectARB  = (PFNGLCREATEPROGRAMOBJECTARBPROC)glfwGetProcAddress("glCreateProgramObjectARB");
  glDeleteObjectARB         = (PFNGLDELETEOBJECTARBPROC)glfwGetProcAddress("glDeleteObjectARB");
  glUseProgramObjectARB     = (PFNGLUSEPROGRAMOBJECTARBPROC)glfwGetProcAddress("glUseProgramObjectARB");
  glCreateShaderObjectARB   = (PFNGLCREATESHADEROBJECTARBPROC)glfwGetProcAddress("glCreateShaderObjectARB");
  glShaderSourceARB         = (PFNGLSHADERSOURCEARBPROC)glfwGetProcAddress("glShaderSourceARB");
  glCompileShaderARB        = (PFNGLCOMPILESHADERARBPROC)glfwGetProcAddress("glCompileShaderARB");
  glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)glfwGetProcAddress("glGetObjectParameterivARB");
  glAttachObjectARB         = (PFNGLATTACHOBJECTARBPROC)glfwGetProcAddress("glAttachObjectARB");
  glGetInfoLogARB           = (PFNGLGETINFOLOGARBPROC)glfwGetProcAddress("glGetInfoLogARB");
  glLinkProgramARB          = (PFNGLLINKPROGRAMARBPROC)glfwGetProcAddress("glLinkProgramARB");
  glGetUniformLocationARB   = (PFNGLGETUNIFORMLOCATIONARBPROC)glfwGetProcAddress("glGetUniformLocationARB");
  glUniform4fARB            = (PFNGLUNIFORM4FARBPROC)glfwGetProcAddress("glUniform4fARB");
  glUniform1fARB            = (PFNGLUNIFORM1FARBPROC)glfwGetProcAddress("glUniform1fARB");
  glUniform1iARB            = (PFNGLUNIFORM1IARBPROC)glfwGetProcAddress("glUniform1iARB");
  
  if( !glActiveTextureARB ||
    !glCreateProgramObjectARB || !glDeleteObjectARB || !glUseProgramObjectARB ||
    !glCreateShaderObjectARB || !glCreateShaderObjectARB || !glCompileShaderARB || 
    !glGetObjectParameterivARB || !glAttachObjectARB || !glGetInfoLogARB || 
    !glLinkProgramARB || !glGetUniformLocationARB || !glUniform4fARB ||
    !glUniform1fARB || !glUniform1iARB )
    {
      printError("GL init error", "One or more GL_ARB_shader_objects functions were not found");
      return;
    }
  */
}


/*
 * readShaderFile - read shader source from a file to a string.
 */
unsigned char * readShaderFile(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if(file == NULL)
    {
      printError("I/O error", "Cannot open shader file!");
      return 0;
    }
  int bytesinfile = filelength(filename);
  unsigned char *buffer = (unsigned char*)malloc(bytesinfile+1);
  int bytesread = fread( buffer, 1, bytesinfile, file);
  buffer[bytesread] = 0; // Terminate the string with 0
  fclose(file);
  
  return buffer;
}


/*
 * createShader - create, load, compile and link the shader object.
 */
GLhandleARB createShader(const char *vertfilename, const char *fragfilename)
{
  
  GLhandleARB programObj;
  GLhandleARB fragmentShader;
  GLhandleARB vertexShader;
  const char *vertexShaderStrings[1];
  GLint vertexCompiled;
  const char *fragmentShaderStrings[1];
  GLint fragmentCompiled;
  GLint shadersLinked;
  char str[4096]; // For error messages from the GLSL compiler and linker
  
  // Create the vertex and fragment shaders
  vertexShader = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
  
  char *vertexShaderAssembly = readShaderFile( vertfilename );
  vertexShaderStrings[0] = vertexShaderAssembly;
  glShaderSourceARB( vertexShader, 1, vertexShaderStrings, NULL );
  glCompileShaderARB( vertexShader );
  free((void *)vertexShaderAssembly);
  
  glGetObjectParameterivARB( vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, 
			     &vertexCompiled );
  if(vertexCompiled == GL_FALSE)
    {
      glGetInfoLogARB( vertexShader, sizeof(str), NULL, str );
      printError("Vertex shader compile error", str);
    }
  
  fragmentShader = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );
  
  char *fragmentShaderAssembly = readShaderFile( fragfilename );
  fragmentShaderStrings[0] = fragmentShaderAssembly;
  glShaderSourceARB( fragmentShader, 1, fragmentShaderStrings, NULL );
  glCompileShaderARB( fragmentShader );
  free((void *)fragmentShaderAssembly);
  
  glGetObjectParameterivARB( fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, 
			     &fragmentCompiled );
  if(fragmentCompiled == GL_FALSE)
    {
      glGetInfoLogARB( fragmentShader, sizeof(str), NULL, str );
      printError("Fragment shader compile error", str);
    }
  
  // Create a program object and attach the compiled shaders
  programObj = glCreateProgramObjectARB();
  glAttachObjectARB( programObj, vertexShader );
  glAttachObjectARB( programObj, fragmentShader );
  
  // Link the program object and print out the info log
  glLinkProgramARB( programObj );
  glGetObjectParameterivARB( programObj, GL_OBJECT_LINK_STATUS_ARB, &shadersLinked );
  
  if( shadersLinked == GL_FALSE )
    {
      glGetInfoLogARB( programObj, sizeof(str), NULL, str );
      printError("Program object linking error", str);
    }

  return programObj;
}


/*
 * setUniformVariables - set the uniform shader variables we need.
 */
 void setUniformVariables(GLhandleARB programObj,
                          int disttexture, int reftexture,
                          float texw, float texh) {

  GLint location_disttexture = -1;
  GLint location_reftexture = -1;
  GLint location_texw = -1;
  GLint location_texh = -1;

  // Activate the shader to set its state
  glUseProgramObjectARB( programObj );
  // Locate the uniform shader variables by name and set them:
  // two textures and two integers to tell the size of the first texture
  location_disttexture = glGetUniformLocationARB( programObj, "disttexture" );
  if(location_disttexture != -1)
    glUniform1iARB( location_disttexture, disttexture );

  location_reftexture = glGetUniformLocationARB( programObj, "reftexture" );
  if(location_reftexture != -1)
    glUniform1iARB( location_reftexture, reftexture );
  
  location_texw = glGetUniformLocationARB( programObj, "texw" );
  if(location_texw != -1)
    glUniform1fARB( location_texw, texw );
  
  location_texh = glGetUniformLocationARB( programObj, "texh" );
  if(location_texh != -1)
    glUniform1fARB( location_texh, texh );

  // Deactivate the shader again
  glUseProgramObjectARB( 0 );
}    


/*
 * loadDistTexture - load 8-bit distance texture data
 * from a TGA file and set up the corresponding texture object.
 */
void loadDistTexture(char *filename, GLuint texID, int *texw, int *texh)
{
  
  //GLFWimage teximg; // Use intermediate GLFWimage to get width and height

  //if(!glfwReadImage(filename, &teximg, GLFW_NO_RESCALE_BIT))
  //  printError("I/O error", "Failed to load distance texture from TGA file.");
  
  //*texw = teximg.Width;
  //*texh = teximg.Height;
  
  glActiveTextureARB(GL_TEXTURE0);
  glBindTexture( GL_TEXTURE_2D, texID );

  //glfwLoadTextureImage2D( &teximg, 0 );
  // The special shader used to render this texture performs its own minification
  // and magnification. Specify nearest neighbor sampling for speed.
  // Using only 8 bits for the disntace encoding, extreme minification is
  // problematic, so a mipmap for minification would actually be useful here.
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //glfwFreeImage(&teximg); // Clean up the malloc()'ed data pointer
}


/*
 * loadRefTexture - Load reference texture data from a TGA file.
 * The reference texture is assumed to be of the same size as the
 * corresponding distance texture. If it isn't, rendering will be wrong.
 */
void loadRefTexture(char *filename, GLuint texID) {
  
  glActiveTextureARB(GL_TEXTURE1);
  glBindTexture( GL_TEXTURE_2D, texID );

  //if(!glfwLoadTexture2D(filename, 0)) // Use GLFW's built-in TGA support 
  //  printError("I/O error", "Failed to load reference texture from TGA file");
    
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


/*
 * showFPS - Calculate and report frames per second (updated once per second)
 * and current magnification level in the window title bar.
 */
void showFPS(int texw, int texh, float zoom) {
  
  static double t0 = 0.0;
  static int frames = 0;
  static char titlestr[200];
  double t, fps;
  
  // Get current time
  //t = glfwGetTime();  // Number of seconds since glfwInit()
  // If one second has passed, or if this is the very first frame
  if( (t - t0) > 1.0 || frames == 0 )
    {
      fps = (double)frames / (t - t0);
      sprintf(titlestr, "%dx%d texture, %.1fx zoom, %.1f FPS", texw, texh, zoom, fps);
      //glfwSetWindowTitle(titlestr);
      t0 = t;
      frames = 0;
    }
  frames ++;
}


/*
 * renderScene - the OpenGL commands to render our scene.
 */
void renderScene(GLhandleARB programObj,
                 float posx, float posy, float zoom, float rotx, float rotz)
{
  int width, height;
  
  // Get window size (may be resized at any time)
  //glfwGetWindowSize( &width, &height );
  if( height<1 ) height = 1;
  if( width<1 ) width = 1;
  
  // Clear color buffer
  glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );
  glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
  glClear( GL_COLOR_BUFFER_BIT );
  
  // Select and set up the projection matrix
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  // Handle interactive zoom by changing FOV (scaling would cause clipping)
  gluPerspective( 45.0f/zoom, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );
  glViewport( 0, 0, width, height );
  
  // Select and set up the modelview matrix
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  
  // Use the shader object for the rendering
  glUseProgramObjectARB( programObj );

  // Handle interactive pan, tilt and rotation
  gluLookAt(10.0f/width*posx, -10.0f/height*posy, 12.0f,
            10.0f/width*posx, -10.0f/height*posy, 0.0f,
            0.0f, 1.0f, 0.0f);
  glRotatef(-rotx, 1.0f, 0.0f, 0.0f);
  glRotatef(rotz, 0.0f, 0.0f, 1.0f);

  // Draw one texture mapped quad in the (x,y) plane
  glBegin( GL_QUADS );
  glTexCoord2f( 0.0f, 1.0f );
  glVertex2f( -5.0f, -5.0f );
  glTexCoord2f( 1.0f, 1.0f );
  glVertex2f( 5.0f, -5.0f );
  glTexCoord2f( 1.0f, 0.0f );
  glVertex2f( 5.0f, 5.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex2f( -5.0f, 5.0f );
  glEnd();
  
  // Deactivate the shader object.
  glUseProgramObjectARB(0);
}


//========================================================================
// main()
//========================================================================

int main(int argc, char *argv[])
{
  // UI-related variables
  int running = 0;
  int mousebtn1, lastmousebtn1;
  int mousex, mousey, lastmousex, lastmousey;
  float posx, posy, zoom, rotx, rotz;

  // Shader-related variables
  GLuint textureID[2];
  int texw = 0;
  int texh = 0;
  GLhandleARB currentProgramObj;
  GLhandleARB programObj[2];
  
  // Initialise GLFW
  //glfwInit();
  
  // Open OpenGL window
  //if( !glfwOpenWindow( 512, 512, 0,0,0,0, 0,0, GLFW_WINDOW ) )
  //  {
  //    glfwTerminate();
  //    return 0;
  //  }

  // Init user interface (mouse drag for pan/zoom/tilt/rotation)
  posx = 0.0f;
  posy = 0.0f;
  zoom = 1.0f;
  rotx = 0.0f;
  rotz = 0.0f;
  //glfwGetMousePos(&mousex, &mousey); // Requires an open window
  lastmousex = mousex;
  lastmousey = mousey;
  //mousebtn1 = lastmousebtn1 = GLFW_RELEASE;
  
  // Load OpenGL extensions (requires an open window)
  loadExtensions();
  
  // Load textures  
  glEnable(GL_TEXTURE_2D);
  glGenTextures( 2, textureID );
  loadDistTexture("dist1.tga", textureID[0], &texw, &texh);
  loadRefTexture("ref1.tga", textureID[1]);
  
  // Create, load and compile the shader programs
  programObj[0] = createShader("vertex.glsl", "fragment1.glsl");
  programObj[1] = createShader("vertex.glsl", "fragment2.glsl");
  currentProgramObj = programObj[0];
  
  // Disable vertical sync (on cards that support
  // it, and if current driver settings so permit)
  glfwSwapInterval( 0 );
  
  // Main loop
  running = GL_TRUE;
  while( running )
    {
      showFPS(texw, texh, zoom);
      
      // Set the uniform shader variables
      setUniformVariables(currentProgramObj, 0, 1, (float)texw, (float)texh);

      renderScene(currentProgramObj, posx, posy, zoom, rotx, rotz);
  /*
      glfwSwapBuffers();
      
      // Handle mouse pan (button 1 drag), zoom (shift-btn 1 drag up/down),
      // tilt (ctrl-btn 1 drag up/down) and rotation (ctrl-btn 1 drag left/right)
      lastmousebtn1 = mousebtn1;
      mousebtn1 = glfwGetMouseButton(GLFW_MOUSE_BUTTON_1);
      lastmousex = mousex;
      lastmousey = mousey;
      glfwGetMousePos(&mousex, &mousey);
      
      if((mousebtn1 == GLFW_PRESS) && (lastmousebtn1 == GLFW_PRESS)) {
        if(glfwGetKey(GLFW_KEY_LSHIFT)) {
          zoom *= pow(1.01, (lastmousey - mousey));
	      if(zoom < 0.26f) zoom = 0.26f; // Do not go beyond 180 degrees FOV
        }
        else if (glfwGetKey(GLFW_KEY_LCTRL)) {
          rotz -= (lastmousex - mousex) * 0.5;
          rotx += (lastmousey - mousey) * 0.5;
      	  if(rotx > 89.5f) rotx = 89.5f;
          if(rotx < 0.0f) rotx = 0.0f;
        }
        else {
      	  posx += (lastmousex - mousex) / zoom;
	      posy += (lastmousey - mousey) / zoom;
        }
      }

      if(glfwGetKey('1')) {
        loadDistTexture("dist1.tga", textureID[0], &texw, &texh);
        loadRefTexture("ref1.tga", textureID[1]);
      }
      if(glfwGetKey('2')) {
        loadDistTexture("dist2.tga", textureID[0], &texw, &texh);
        loadRefTexture("ref2.tga", textureID[1]);
      }
      if(glfwGetKey('3')) {
        loadDistTexture("dist3.tga", textureID[0], &texw, &texh);
        loadRefTexture("ref3.tga", textureID[1]);
      }
      if(glfwGetKey('4')) {
        loadDistTexture("dist4.tga", textureID[0], &texw, &texh);
        loadRefTexture("ref4.tga", textureID[1]);
      }
      if(glfwGetKey(GLFW_KEY_F1)) {
        currentProgramObj = programObj[0];
      }
      if(glfwGetKey(GLFW_KEY_F2)) {
        currentProgramObj = programObj[1];
      }

      // Check if the ESC key is pressed or the window has been closed
      running = !glfwGetKey( GLFW_KEY_ESC ) &&
       glfwGetWindowParam( GLFW_OPENED );
    }
  
  */
  
  // Close the window (if still open) and terminate GLFW
  //glfwTerminate();
  
  return 0;
}
