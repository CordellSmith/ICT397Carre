#shader vertex
#version 400

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec4 frag_Color;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
	frag_Color = in_Color;
}


#shader fragment
#version 400

in vec4 frag_Color;
layout(location = 0) out vec4 out_Color;

void main(void)
{
	out_Color = frag_Color;
}