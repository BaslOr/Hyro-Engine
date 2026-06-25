#version 460 core

out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;
in float o_SpriteIndex;

uniform sampler2D[] u_Textures;

void main()
{
	FragColor = texture(u_Textures[int(o_SpriteIndex)], o_UV);
}
