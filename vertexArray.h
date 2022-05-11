#pragma once

#include "vertexBuffer.h"

class vertexArray{

    private:



    public:
    
    vertexArray();
    ~vertexArray();

    void addBuffer(const vertexBuffer& vb, const vertexBufferLayout& layout);

}