#version 460 core
in vec2 texcoords;

out vec4 fragColor;

uniform sampler2D u_screenTexture;

void main()
{
    vec3 color = texture(u_screenTexture, texcoords).rgb;
    fragColor = vec4(color, 1.0);
}
