#version 450

#ifdef VULKAN
layout(push_constant) uniform PushConstants {
    mat4 mvp;
} u_PushConstants;
#define u_Model u_PushConstants.mvp
#else
uniform mat4 u_Model;
#endif

struct PerVertex {
    vec2 uv;
    vec3 worldNormal;
    vec3 worldPos;
};

layout(location = 0) in vec3 a_Position;

layout(location = 0) out vec3 o_UV;

void main() {
    o_UV = a_Position;

    vec4 pos = u_Model * vec4(a_Position, 1.0f);
    gl_Position = pos.xyww;  // z = w → Tiefe wird immer 1.0
}
