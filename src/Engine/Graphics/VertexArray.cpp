#include "VertexArray.hpp"
#include <glad/glad.h>

namespace Hollow {

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {}

void VertexArray::Destroy()
{
    if (m_ID != 0)
    {
        glDeleteVertexArrays(1, &m_ID);
        m_ID = 0;
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& e = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            e.count,
            e.type,
            e.normalized,
            layout.GetStride(),
            (const void*)(uintptr_t)offset
        );

        offset += e.count * BufferElement::GetSizeOfType(e.type);
    }
}

}