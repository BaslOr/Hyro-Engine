#version 460 core  
layout (location = 0) in vec3 aPos;  
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aSpriteIndex;

uniform mat4 u_ProjectionMatrix;

out vec4 o_Color;
out vec2 o_UV;
out float o_SpriteIndex;
		 
void main()  {  
	o_Color = aColor;
	o_UV = aUV;
	o_SpriteIndex = aSpriteIndex;
    gl_Position = u_ProjectionMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);  
}
