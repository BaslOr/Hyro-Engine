#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inUV;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 fragUV;

layout(std140, binding = 0) uniform UniformBufferObject {
    mat4 MVP;
} ubo;


#ifdef VULKAN
layout(push_constant) uniform PushConstants {
    mat4 model;
} u_PushConstants;
#define u_Model u_PushConstants.model
#else
uniform mat4 u_Model;
#endif

void main() {
    gl_Position = ubo.MVP * u_Model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragUV = inUV;
}