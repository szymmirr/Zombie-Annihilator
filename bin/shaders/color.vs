#version 400
layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_texCoord;
layout(location=3) in vec3 in_Normal;
out vec4 ex_Color;
out vec2 TexCoord;
out vec3 Normal;
out vec4 xPosition;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 LightPosition;
uniform vec3 EyePosition;

void main(void)
{
	mat4 wvp = Projection * View;
	ex_Color = in_Color;
	TexCoord = in_texCoord;
	Normal = normalize(mat3x3(World)*in_Normal);
	xPosition = World * vec4(in_Position.xyz, 1);
	gl_Position = Projection * View * xPosition;
}