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
};

uniform light light0;

invariant in vec3 a_Vertex;
in vec2 a_TexCoord0;
in float a_TexCoord1;

in vec3 a_Normal;

out vec4 color;
out vec2 texCoord0;
out float texCoord1;

void main(void) 
{
	vec3 N = normalize(normal_matrix * a_Normal);	
	vec3 L = normalize(modelview_matrix * light0.position).xyz;
	float NdotL = max(dot(N, L.xyz), 0.0);

	vec4 finalColor = material_ambient * light0.ambient;
	vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);	
	vec3 E = -pos.xyz;

	if (NdotL > 0.0) 
	{
		vec3 HV = normalize(L + E);
		finalColor += material_diffuse * light0.diffuse * NdotL;

		float NdotHV = max(dot(N, HV), 0.0);
		color += material_specular * light0.specular * pow(NdotHV, material_shininess);	
	}

	color = finalColor;
	texCoord0 = a_TexCoord0;
	texCoord1 = a_TexCoord1;

	gl_Position = projection_matrix * pos;	
}
