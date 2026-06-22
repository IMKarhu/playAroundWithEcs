#version 460 core
layout(location = 0) in vec2 a_position;
layout(location = 2) in vec2 a_texcoord;

out vec2 texcoords;

void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
    texcoords = a_texcoord;
}
