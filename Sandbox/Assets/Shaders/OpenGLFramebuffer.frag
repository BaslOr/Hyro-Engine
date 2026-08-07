#version 460
layout(location = 0) out vec4 FragColor;
  
layout(location = 0) in vec2 TexCoords;

layout(set = 0, binding = 0) uniform sampler2D screenTexture;

void main()
{ 
    FragColor = texture(screenTexture, TexCoords);
}