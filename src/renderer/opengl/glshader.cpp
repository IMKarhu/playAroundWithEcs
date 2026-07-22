#include "glshader.h"
#include <print>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <memory>
#include <glm/gtc/type_ptr.hpp>

namespace Lumos
{
    GLShader::GLShader(std::string vertexshader, std::string fragmentshader)
    {
        if(std::filesystem::exists(vertexshader.data())) {
            std::println("this infact does exist");
        }
        uint32_t vertexid;
        uint32_t fragmentid;
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
        vertexid = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexid, 1, &vShaderCode, nullptr);
        glCompileShader(vertexid);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexid, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexid, 512, nullptr, infoLog);
            std::cout << "Error Shader Vertex compilation failed!\n" << infoLog << std::endl;
        }
        fragmentid = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentid, 1, &fShaderCode, nullptr);
        glCompileShader(fragmentid);
        glGetShaderiv(fragmentid, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentid, 512, nullptr, infoLog);
            std::cout << "Error Shader fragment compilation failed!\n" << infoLog << std::endl;
        }
        m_shaderprogram = glCreateProgram();
        glAttachShader(m_shaderprogram, vertexid);
        glAttachShader(m_shaderprogram, fragmentid);
        glLinkProgram(m_shaderprogram);
        glGetProgramiv(m_shaderprogram, GL_LINK_STATUS, &success);
        GLint uniformcount = 0;
        if (!success)
        {
            glGetProgramInfoLog(m_shaderprogram, 512, NULL, infoLog);
        }
        glGetProgramiv(m_shaderprogram, GL_ACTIVE_UNIFORMS, &uniformcount);
        if (uniformcount != 0) {
            std::println("active uniforms count: {}", uniformcount);
            GLint namelen = 0;
            GLsizei length = 0;
            GLsizei count = 0;
            GLenum type = GL_NONE;
            glGetProgramiv(m_shaderprogram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &namelen);
            auto name = std::make_unique<char[]>(namelen);
            for (GLint i = 0; i < uniformcount; ++i) {
                glGetActiveUniform(m_shaderprogram, i, namelen, &length, &count, &type, name.get());
                UniformInfo info{};
                info.loc = glGetUniformLocation(m_shaderprogram, name.get());
                info.count = count;
                m_uniforms.emplace(std::make_pair(std::string(name.get(), length), info));
            }
        }
        glDeleteShader(vertexid);
        glDeleteShader(fragmentid);
    }

    GLShader::~GLShader() {}

    void GLShader::bind()
    {
        glUseProgram(m_shaderprogram);
    }

    void GLShader::unbind()
    {
        glUseProgram(0);
    }

    void GLShader::setUniformMat4(const std::string& name, const glm::mat4& mat4)
    {
        auto it = m_uniforms.find(name);
        if(it != m_uniforms.end()) {
            glProgramUniformMatrix4fv(m_shaderprogram,
                                      it->second.loc,
                                      it->second.count,
                                      GL_FALSE,
                                      glm::value_ptr(mat4));
        }
    }
    void GLShader::setUniformVec3(const std::string& name, const glm::vec3& vec3)
    {
        auto it = m_uniforms.find(name);
        if(it != m_uniforms.end()) {
            glProgramUniform3fv(m_shaderprogram,
                                it->second.loc,
                                it->second.count,
                                glm::value_ptr(vec3));
        }
    }

    void GLShader::setUniformTexture(const std::string& name, int value)
    {
        auto it = m_uniforms.find(name);
        if(it != m_uniforms.end()) {
            glProgramUniform1i(m_shaderprogram,it->second.loc,value);
        }
    }

    void GLShader::setUniformInt(const std::string& name, const int& value)
    {
        auto it = m_uniforms.find(name);
        if(it != m_uniforms.end()) {
            glProgramUniform1i(m_shaderprogram, it->second.loc, value);
        }
    }

    void GLShader::setUniformfloat(const std::string& name, const int& value)
    {
        auto it = m_uniforms.find(name);
        if(it != m_uniforms.end()) {
            glProgramUniform1f(m_shaderprogram, it->second.loc, value);
        }
    }
}// namespace Lumos

