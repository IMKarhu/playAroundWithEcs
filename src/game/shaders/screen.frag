#version 460 core
in vec2 texcoords;

out vec4 fragColor;

layout(binding = 0) uniform sampler2D scenetexture;

void main()
{
    vec3 color = texture(scenetexture, texcoords).rgb;
    fragColor = vec4(color, 1.0);
}
