#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec2 texCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(vPosition, 1.0);
	
	TexCoord = vec2( texCoord.x, 1.0 - texCoord.y );
}