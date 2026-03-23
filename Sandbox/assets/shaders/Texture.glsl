#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = viewProjection * transform * vec4(aPosition, 1.0);
}



#type fragment
#version 330 core

layout(location = 0) out vec4 fColor;

uniform sampler2D ourTexture;

in vec2 TexCoord;

void main()
{
	fColor = texture(ourTexture, TexCoord);
}
