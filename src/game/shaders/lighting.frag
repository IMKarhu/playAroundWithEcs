#version 460 core
in vec2 texcoords;

out vec4 fragColor;

layout(binding = 0) uniform sampler2D galbedo;
layout(binding = 1) uniform sampler2D gnormal;
layout(binding = 2) uniform sampler2D gposition;

struct Light
{
    vec3 position;
    vec3 lightdirection;
    vec3 color;
    float intensity;
    float radius;
};

uniform vec3 u_viewpos;
uniform Light u_lights;

void main()
{
    vec3 albedo = texture(galbedo, texcoords).rgb;
    vec3 normal = texture(gnormal, texcoords).rgb;
    vec3 gposition = texture(gposition, texcoords).rgb;
    float specular_float = texture(galbedo, texcoords).a;

    vec3 V = normalize(u_viewpos - gposition);
    // vec3 L = normalize(u_lights.position - gposition);
    vec3 L = normalize(-u_lights.lightdirection);
    vec3 H = normalize(L + V);

    float specstr = 0.5;
    float ambientstr = 0.1;

    vec3 ambient = ambientstr * u_lights.color;
    float specular = pow(max(dot(normal, H), 0.0), 32);
    vec3 spec = specular * u_lights.color;

    float diffuse = max(dot(normal, L), 0.0);
    vec3 dif = diffuse * u_lights.color;
    vec3 result = (ambient + dif + spec) * albedo;
    fragColor = vec4(result, 1.0);
}
