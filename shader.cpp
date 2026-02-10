#include "shader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexshader, const char* fragmentshader)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexshader);
        fShaderFile.open(fragmentshader);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << e.code() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    m_vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexshader, 1, &vShaderCode, nullptr);
    glCompileShader(m_vertexshader);
    int success;
    char infoLog[512];
    glGetShaderiv(m_vertexshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_vertexshader, 512, nullptr, infoLog);
        std::cout << "Error Shader Vertex compilation failed!\n" << infoLog << std::endl;
    }
    m_fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentshader, 1, &fShaderCode, nullptr);
    glCompileShader(m_fragmentshader);
    glGetShaderiv(m_fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_fragmentshader, 512, nullptr, infoLog);
        std::cout << "Error Shader fragment compilation failed!\n" << infoLog << std::endl;
    }
    m_shaderprogram = glCreateProgram();
    glAttachShader(m_shaderprogram, m_vertexshader);
    glAttachShader(m_shaderprogram, m_fragmentshader);
    glLinkProgram(m_shaderprogram);
    glGetProgramiv(m_shaderprogram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shaderprogram, 512, NULL, infoLog);
    }
    glDeleteShader(m_vertexshader);
    glDeleteShader(m_fragmentshader);
}

void Shader::use()
{
    glUseProgram(m_shaderprogram);
}
// void setUniformVec4(const std::string& name, const glm::vec4& vec4);
void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat4)
{
    int location = glGetUniformLocation(m_shaderprogram, name.c_str());
    if (location < 0)
    {
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));

}
void Shader::setUniformVec3(const std::string& name, const std::array<float, 3>& vec3)
{
    int location = glGetUniformLocation(m_shaderprogram, name.c_str());
    if (location < 0)
    {
        return;
    }
    glUniform3fv(location, 1, vec3.data());
}

void Shader::setUniformTexture(const std::string& name, int value)
{
    int location = glGetUniformLocation(m_shaderprogram, name.c_str());
    if (location < 0)
    {
        return;
    }
    glUniform1i(location, value);
}

void Shader::setUniformInt(const std::string& name, const int& value)
{
    int location = glGetUniformLocation(m_shaderprogram, name.c_str());
    if (location < 0)
    {
        return;
    }
    glUniform1i(location, value);
}

