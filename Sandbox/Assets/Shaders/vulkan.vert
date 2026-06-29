#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;
layout(location = 3) in float inSpriteIndex;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out float fragSpriteIndex;

layout(std140, binding = 0) uniform UniformBufferObject {
    mat4 Projection;
} ubo;


void main() {
    gl_Position = ubo.Projection * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragSpriteIndex = inSpriteIndex;
}