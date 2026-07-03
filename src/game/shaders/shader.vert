#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 u_mvp;

out vec4 color;
out vec2 texcoord;

void main()
{
    gl_Position = u_mvp * vec4(a_position, 1.0);
    color = a_color;
    texcoord = a_texcoord;
}
