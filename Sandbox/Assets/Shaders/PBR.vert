#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragUV;
layout(location = 2) out vec3 fragNormal;

layout(std140, binding = 0) uniform Transform {
    mat4 MVP;
} transform;


//Map push constants to a uniform variable for OpenGL
#ifdef VULKAN
layout(push_constant) uniform PushConstants {
    mat4 model;
} u_PushConstants;
#define u_Model u_PushConstants.model
#else
uniform mat4 u_Model;
#endif

void main() {
    fragUV = inUV;
    fragPosition = vec3(u_Model * vec4(inPosition, 1.0));
    fragNormal = mat3(transpose(inverse(u_Model))) * inNormal;

    gl_Position = transform.MVP * u_Model * vec4(inPosition, 1.0);
}
