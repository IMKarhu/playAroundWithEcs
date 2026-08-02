#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <string>
#include <unordered_map>
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
            void setUniformUint32(const std::string& name, const uint32_t& value);
            void setUniformfloat(const std::string& name, const int& value);

        private:
            struct UniformInfo
            {
                GLint loc;
                GLsizei count;
            };
            uint32_t m_shaderprogram;
            std::unordered_map<std::string, UniformInfo> m_uniforms;
    };
}// namespace Lumos

