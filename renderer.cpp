#include "renderer.h"


using namespace std;


void GLClearError(){
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * function, const char * file, int line){
    if (GLenum error = glGetError()){
        cout << "[OpenGL Error] in file " << file << " and function " << function << " on line " << line << " : "<< error << endl;
        return false;
    } 
    return true;
}
