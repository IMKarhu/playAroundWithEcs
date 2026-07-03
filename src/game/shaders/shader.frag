#version 460 core
in vec4 color;
in vec2 texcoord;

out vec4 fragColor;

uniform sampler2D ubasecolortexture;
uniform int uusetexture;

void main()
{
    if(uusetexture > 0) {
        vec4 texColor = texture(ubasecolortexture, texcoord);
        fragColor = texColor;
    }
    else
    {
        fragColor = vec4(color);
    }
}
