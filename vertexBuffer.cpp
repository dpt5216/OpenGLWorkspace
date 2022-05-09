#include "vertexBuffer.h"

vertexBuffer::vertexBuffer(const void* data, unsigned int size){
    GL_CALL(glGenBuffers(1, &m_RendererID)); 
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); 
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), data, GL_STATIC_DRAW)); //GL_STATIC_DRAW is just a hint for the implementation, can do other behavior with it but may be slow
}

vertexBuffer::~vertexBuffer(){
    GL_CALL(glDeleteBuffers(1, &m_RendererID));
}

void vertexBuffer::bind(){
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void vertexBuffer::unbind(){
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}