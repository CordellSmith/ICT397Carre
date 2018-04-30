#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;

out vec4 VertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * aPos;
	VertexColor = aColor;
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	//TexCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 VertexColor;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
	//FragColor = texture(texture_diffuse1, TexCoord);
	FragColor = VertexColor;
}