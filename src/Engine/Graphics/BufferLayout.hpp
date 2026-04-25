#pragma once
#include <vector>

namespace Hollow {

struct BufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case 0x1406: return 4; // GL_FLOAT
        }
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() : m_Stride(0) {}

    void PushFloat(unsigned int count)
    {
        m_Elements.push_back({0x1406, count, false});
        m_Stride += count * 4;
    }

    const std::vector<BufferElement>& GetElements() const { return m_Elements; }
    unsigned int GetStride() const { return m_Stride; }

private:
    std::vector<BufferElement> m_Elements;
    unsigned int m_Stride;
};

}