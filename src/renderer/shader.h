#pragma once

namespace Lumos
{
    class Shader
    {
        public:
            virtual ~Shader() = default;
            virtual void bind() = 0;
            virtual void unbind() = 0;
        private:
    };
}// namespace Lumos

