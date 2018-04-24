#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;

//out vec2 TexCoords;
out vec4 frag_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	frag_Color = aColor;
}

#shader fragment
#version 330 core
//out vec4 FragColor;

//in vec2 TexCoords;
in vec4 frag_Color;

//uniform sampler2D texture_diffuse1;
layout(location = 0) out vec4 out_Color;

void main()
{
	out_Color = frag_Color;
    //FragColor = texture(texture_diffuse1, TexCoords);
}