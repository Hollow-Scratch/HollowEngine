#pragma once
#include "VertexBuffer.hpp"
#include "BufferLayout.hpp"

namespace Hollow {

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer& vb, const BufferLayout& layout);

private:
    unsigned int m_ID = 0;
};

}