#version 460 core  
layout (location = 0) in vec3 aPos;  
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

uniform mat4 u_ProjectionMatrix;

out vec4 o_Color;
out vec2 o_UV;
		 
void main()  {  
	o_Color = aColor;
	o_UV = aUV;
    gl_Position = u_ProjectionMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);  
}
