#pragma once

#include <string>
#include <unordered_map>

namespace Hollow {

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    void Bind() const;
    void Unbind() const;


    void SetFloat(const std::string& name, float value);
    void SetInt(const std::string& name, int value);
    void SetVec3(const std::string& name, float x, float y, float z);
    void SetMat4(const std::string& name, const float* value);

private:
    unsigned int m_ID = 0;

    std::string ReadFile(const std::string& path);
    unsigned int Compile(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name);

private:
    std::unordered_map<std::string, int> m_UniformCache;
};

}