#include "indexBuffer.h"

indexBuffer::indexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    

    GL_CALL(glGenBuffers(1, &m_RendererID)); 
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); 
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * 2 * sizeof(float), data, GL_STATIC_DRAW)); //GL_STATIC_DRAW is just a hint for the implementation, can do other behavior with it but may be slow
}

indexBuffer::~indexBuffer(){
    GL_CALL(glDeleteBuffers(1, &m_RendererID));
}

void indexBuffer::bind(){
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void indexBuffer::unbind(){
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}