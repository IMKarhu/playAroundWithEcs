#version 460 core
in vec2 texcoords;

out vec4 fragColor;

layout(binding = 0) uniform sampler2D galbedo;
layout(binding = 1) uniform sampler2D gnormal;
layout(binding = 2) uniform sampler2D gposition;

uniform vec3 u_lightdir;
uniform vec3 u_lightcolor;

void main()
{
    vec3 albedo = texture(galbedo, texcoords).rgb;
    vec3 normal = texture(gnormal, texcoords).rgb;
    vec3 gposition = texture(gposition, texcoords).rgb;

    //calculate some really basic lighting
    vec3 ambient = 0.1 * albedo;
    vec3 lightdir = normalize(-u_lightdir);
    vec3 diffuse = max(dot(normal, lightdir), 0.0) * albedo * u_lightcolor;
    fragColor = vec4(diffuse + ambient, 1.0);
}
