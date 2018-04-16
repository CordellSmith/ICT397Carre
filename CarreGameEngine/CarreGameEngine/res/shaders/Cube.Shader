#shader vertex
#version 400

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec4 normals;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

//out vec4 frag_Color;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(void)
{
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * in_Position;
	//frag_Color = in_Color;
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

#shader fragment
#version 400

//in vec4 frag_Color;
//layout(location = 0) out vec4 out_Color;

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(void)
{
	out_Color = frag_Color;
	// Output color = color of the texture at the specified UV
	color = texture(myTextureSampler, UV).rgb;
}