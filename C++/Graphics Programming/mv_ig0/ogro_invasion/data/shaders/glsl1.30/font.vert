#version 130

uniform mat4 modelview_matrix;
uniform mat4 projection_matrix;

in vec2 a_Vertex;
in vec2 a_TexCoord0;
out vec2 texCoord0;

void main(void) {
	texCoord0 = a_TexCoord0;
	
	vec4 pos = modelview_matrix * vec4(a_Vertex, 0.0, 1.0);	
	gl_Position = projection_matrix * pos;	
}
