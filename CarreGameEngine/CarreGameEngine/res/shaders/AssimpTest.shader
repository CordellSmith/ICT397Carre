#shader vertex
#version 330 core
layout(location = 0) in glm::vec3 aPos;
layout(location = 1) in glm::vec3 aNormal;
layout(location = 2) in glm::vec2 aTexCoords;

out glm::vec2 TexCoords;

uniform glm::mat4 model;
uniform glm::mat4 view;
uniform glm::mat4 projection;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * glm::vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out glm::vec4 FragColor;

in glm::vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}