#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform float offsetx;
uniform float offsety;

uniform int textureActive = 1;

void main(){
	if(textureActive == 1){
		vec4 tex1 = texture(texture1, vec2(TexCoord.x + offsetx, TexCoord.y + offsety));
		if(tex1.a < 0.5) {
			discard;
		}
		color = tex1;
	}
	else if(textureActive == 2){
		vec4 tex1 = texture(texture2, vec2(TexCoord.x + offsetx, TexCoord.y + offsety));
		if(tex1.a < 0.5) {
			discard;
		}
		color = tex1;
	}
	else
	{
		vec4 tex1 = texture(texture3, vec2(TexCoord.x + offsetx, TexCoord.y + offsety));
		if(tex1.a < 0.5) {
			discard;
		}
		color = tex1;
	}
}