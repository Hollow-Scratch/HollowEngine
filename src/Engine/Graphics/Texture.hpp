#pragma once

namespace Hollow {

class Texture
{
public:
    Texture(const char* path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void Destroy();

private:
    unsigned int m_ID = 0;
};

}