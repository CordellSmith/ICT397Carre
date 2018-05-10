#shader vertex
#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

out vec4 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 0.5f);
	VertexColor = inColor;
}

#shader fragment
#version 330 core

in vec4 VertexColor;

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = VertexColor;
}