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

out vec4 color;
out vec2 texCoord0;

void main(void) 
{
    vec3 N = normalize(normal_matrix * a_Normal);	
    vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);

    //Calculate the light position in eye space by multiplying by the modelview matrix
    vec3 lightPos = (modelview_matrix * light0.position).xyz;
    //Get the light direction vector by finding the 
    //vector between the vertex and light position
    vec3 lightDir = (lightPos - pos.xyz).xyz;

    //Find the angle between the normal and the light direction
    float NdotL = max(dot(N, lightDir.xyz), 0.0);
    //The distance between the vertex and the light is the length
    //of the light direction vector
    float dist = length(lightDir);

    //Find the eye vector (same as a directional light)
    vec3 E = -(pos.xyz);

    //Assign the ambient color
    vec4 finalColor = material_ambient * light0.ambient;

    if (NdotL > 0.0) 
    {
        vec3 HV = normalize(lightPos + E);
        finalColor += material_diffuse * light0.diffuse * NdotL;

	  float NdotHV = max(dot(N, HV), 0.0);
        finalColor += material_specular * light0.specular * pow(NdotHV, material_shininess);	
    }
	
	//Calculate the attenuation factor
	float attenuation = 1.0 / (light0.constant_attenuation + 
                                 light0.linear_attenuation * dist + 
                                 light0.quadratic_attenuation * dist * dist);

	//The material emissive value isn't affected by attenuation so that is added separately
	color = material_emissive + (finalColor * attenuation);

	texCoord0 = a_TexCoord0;
	gl_Position = projection_matrix * pos;	
}
