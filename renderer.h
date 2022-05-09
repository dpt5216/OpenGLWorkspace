
#ifndef RENDERER
#define RENDERER

#include <iostream>
#include <assert.h>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>



#define GL_CALL(x) GLClearError();\
    x;\
    assert(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char * function, const char * file, int line);

#endif