#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D u_Textures[16];

void main() {
    outColor = texture(u_Textures[1], fragUV.xy);
}