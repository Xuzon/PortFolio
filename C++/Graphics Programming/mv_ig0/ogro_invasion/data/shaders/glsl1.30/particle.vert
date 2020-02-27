#version 130

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform float point_size;

in vec3 a_Vertex;
in vec4 a_Color;

out vec4 color;

void main(void) 
{
	vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);	
	color = a_Color;
	
	gl_PointSize = point_size; //2 * point_size / -pos.z;
	gl_Position = projection_matrix * pos;		
}
