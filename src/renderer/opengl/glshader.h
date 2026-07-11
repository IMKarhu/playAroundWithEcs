#pragma once
#include "shader.h"
#include <string>
#include <glm/glm.hpp>

namespace Lumos
{
    class GLShader : public Shader
    {
        public:
            GLShader(std::string vertexshader, std::string fragmentshader);
            ~GLShader() override;

            void bind() override;
            void unbind() override;
            void setUniformMat4(const std::string& name, const glm::mat4& mat4);
            void setUniformVec3(const std::string& name, const glm::vec3& vec3);
            void setUniformTexture(const std::string& name, int value);
            void setUniformInt(const std::string& name, const int& value);
        private:
            uint32_t m_shaderprogram;
    };
}// namespace Lumos

