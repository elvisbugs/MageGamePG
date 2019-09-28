#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform int textureActive = 1;

void main(){
	if(textureActive == 1){
		vec4 tex1 = texture(texture1, TexCoord );
		color = tex1;
	}
	else if(textureActive == 2){
		vec4 tex1 = texture(texture2, TexCoord );
		color = tex1;
	}
	else
	{
		vec4 tex1 = texture(texture3, TexCoord );
		color = tex1;
	}
}