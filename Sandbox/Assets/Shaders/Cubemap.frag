#version 450

layout(location = 0) in vec3 i_UV;
layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform samplerCube u_Cube;

void main() {
    o_Color = texture(u_Cube, i_UV);
}