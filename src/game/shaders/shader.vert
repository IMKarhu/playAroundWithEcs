#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;
layout(location = 3) in vec4 a_tangents;

uniform mat4 u_model;
uniform mat4 u_viewproj;

out vec3 position; //in world coordinates
out vec2 texcoord;
out vec3 normal;
out mat3 tbn;

void main()
{
    vec4 worldpos = u_model * vec4(a_position, 1.0);
    position = worldpos.xyz;
    texcoord = a_texcoord;
    normal = a_normal;

    vec3 T = normalize(vec3(u_model * vec4(a_tangents.xyz, 0.0)));//worldspace
    vec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));//worldspace
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T) * a_tangents.w;//calculate bitangent vector
    tbn = mat3(T, B, N);

    gl_Position = u_viewproj * worldpos;
}
