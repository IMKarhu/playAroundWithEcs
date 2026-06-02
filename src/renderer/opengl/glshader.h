#pragma once
#include "shader.h"
#include <string>
#include <array>
#include <glm/glm.hpp>

class GLShader : public Shader
{
public:
    GLShader(std::string vertexshader, std::string fragmentshader);
    ~GLShader() override;

    void bind() override;
    void unbind() override;
    void setUniformMat4(const std::string& name, const glm::mat4& mat4);
    void setUniformVec3(const std::string& name, const std::array<float, 3>& vec3);
    void setUniformTexture(const std::string& name, int value);
    void setUniformInt(const std::string& name, const int& value);
private:
    uint32_t m_vertexshader;
    uint32_t m_fragmentshader;
    uint32_t m_shaderprogram;
};
