#pragma once

namespace Hollow {

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    void Destroy();

    unsigned int GetCount() const;

private:
    unsigned int m_ID = 0;
    unsigned int m_Count = 0;
};

}