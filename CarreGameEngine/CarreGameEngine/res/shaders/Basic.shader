#shader vertex
#version 400

layout(location = 0) in glm::vec4 in_Position;
layout(location = 1) in glm::vec4 in_Color;

uniform glm::mat4 u_projectionMatrix;
uniform glm::mat4 u_viewMatrix;
uniform glm::mat4 u_modelMatrix;

out glm::vec4 frag_Color;

void main(void)
{
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * in_Position;
	frag_Color = in_Color;
}

#shader fragment
#version 400

in glm::vec4 frag_Color;
layout(location = 0) out glm::vec4 out_Color;

void main(void)
{
	out_Color = frag_Color;
}