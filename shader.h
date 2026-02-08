#pragma once
#include <string>
#include <array>

class Shader
{
public:
    Shader(const char* vertexshader, const char* fragmentshader);
    ~Shader();
    void use();
    // void setUniformVec4(const std::string& name, const glm::vec4& vec4);
    // void setUniformMat4(const std::string& name, const glm::mat4& mat4);
    void setUniformVec3(const std::string& name, const std::array<float, 3>& vec3);
    void setUniformTexture(const std::string& name, int value);
    void setUniformInt(const std::string& name, const int& value);
private:
    uint32_t m_vertexshader;
    uint32_t m_fragmentshader;
    uint32_t m_shaderprogram;
};
