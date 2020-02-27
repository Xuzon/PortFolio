#version 130

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emissive;
uniform float material_shininess;

struct light {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;

	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
};

uniform light light0;

in vec3 a_Vertex;
in vec2 a_TexCoord0;
in vec3 a_Normal;

out vec2 texCoord0;
out vec3 normal;
out vec3 lightDir;
out vec3 halfVector;

out vec4 vertDiffuse;
out vec4 vertAmbient;
out float dist;

void main(void) 
{
    normal = normalize(normal_matrix * a_Normal);	
    vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);

    //Calculate the light position in eye space by multiplying by the modelview matrix
    vec3 lightPos = (modelview_matrix * light0.position).xyz;
    //Get the light direction vector by finding the 
    //vector between the vertex and light position
    lightDir = lightPos - pos.xyz;
    //Find the eye vector (same as a directional light)
    vec3 E = -(pos.xyz);
	
	
	dist = length(lightDir);
	lightDir = normalize(lightDir);
	
    halfVector = normalize(lightPos + E);
	
	vertDiffuse = material_diffuse * light0.diffuse;
	vertAmbient = material_ambient * light0.ambient;
	texCoord0 = a_TexCoord0;
	
	gl_Position = projection_matrix * pos;	
}
