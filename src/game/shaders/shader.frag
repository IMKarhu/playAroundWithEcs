#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require
layout(location = 0) out vec4 galbedo;
layout(location = 1) out vec3 gnormal;
layout(location = 2) out vec3 gposition;

in vec3 position;
in vec2 texcoord;
in vec3 normal;
in mat3 tbn;

struct GpuMaterial
{
    uint64_t baseColorHandle;
    uint64_t normalHandle;
    uint64_t metallicRoughnessHandle;

    vec4 baseColorFactor;

    float metallicFactor;
    float roughnessFactor;

    uint flags;
};

layout(std430, binding = 0) readonly buffer Materialbuffer
{
    GpuMaterial materials[];
}

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
    galbedo.a = 1.0f; //specular
}
