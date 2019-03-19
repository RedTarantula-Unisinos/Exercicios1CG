#shader vertex
#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_colour;
layout(location = 2) in vec2 aTexCoord;


out vec3 colour;
out vec2 TexCoord;
void main()
{
	colour = vertex_colour;
	gl_Position = vec4(vertex_position, 1.0);
	TexCoord = aTexCoord;
}
#shader fragment
#version 330 core

in vec3 colour;
in vec2 TexCoord;
uniform sampler2D ourTexture;
out vec4 frag_colour;
void main()
{
	frag_colour = texture(ourTexture, TexCoord);
}	