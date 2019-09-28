#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 transform;
uniform bool doTransform = false;

void main()
{
	if(doTransform)
	{
		gl_Position = transform * vec4(vPosition, 1.0);
	}
	else
	{
		gl_Position = vec4(vPosition, 1.0);
	}
	
	TexCoord = vec2( texCoord.x, 1.0 - texCoord.y );
}