#version 460 core

out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;

void main()
{
	FragColor = vec4(o_Color);
}
