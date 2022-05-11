#pragma once
#include <vector>
#include <GL/glew.h>

struct vertexBufferElement{
    unsigned int count;
    unsigned int type;
    bool normalized;
};

class vertexBufferLayout{
    private:
    std::vector<vertexBufferElement> m_elements;

    public:
    vertexBufferLayout();
    ~vertexBufferLayout();

    template<typename T>
    void push(int count){
        static_assert(false);
    }

    template<>
    void push<float>(int count){
        m_elements.push_back({GL_FLOAT, count, false});
    }

}