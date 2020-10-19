#version 400
layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_texCoord;
out vec2 TexCoord;
out vec4 xPosition;
uniform mat4x4 World;
void main(void)
{
	xPosition = vec4(in_Position.xyz, 1);
	TexCoord = xPosition.xy;
	TexCoord.x = (1+xPosition.x)*0.5f;
	TexCoord.y = (1+xPosition.y)*0.5f;
	gl_Position = World * xPosition;
}