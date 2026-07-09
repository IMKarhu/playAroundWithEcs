#version 460 core
layout(location = 0) out vec4 galbedo;
layout(location = 1) out vec3 gnormal;
layout(location = 2) out vec3 gposition;

in vec3 position;
in vec2 texcoord;
in vec3 normal;
in mat3 tbn;

layout(binding = 0) uniform sampler2D ubasecolortexture;
layout(binding = 1) uniform sampler2D unormaltexture;
layout(binding = 2) uniform sampler2D umetallicroughnesstexture;

void main()
{
    gposition = position;
    vec3 localnormal = texture(unormaltexture, texcoord).rgb;
    localnormal = localnormal * 2.0 - 1.0;
    vec3 worldnormal = normalize(tbn * localnormal);
    gnormal = worldnormal;
    galbedo.xyz = texture(ubasecolortexture, texcoord).rgb;
    galbedo.a = 1.0f; //roughness, can be hardcoded to 1.0 for now
}
