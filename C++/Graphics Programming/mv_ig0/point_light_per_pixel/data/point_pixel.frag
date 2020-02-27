#version 130

precision highp float;

uniform sampler2D texture0;

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

in vec2 texCoord0;
in vec3 normal;
in vec3 lightDir;
in vec3 halfVector;

in vec4 vertDiffuse;
in vec4 vertAmbient;
in float dist;

out vec4 outColor;

void main(void) {
	vec3 N = normalize(normal); //Renormalize the normal
	
	//Find the angle between the normal and the light
	float NdotL = max(dot(N, normalize(lightDir)), 0.0);
	
	//Start off with black
	vec4 color = vec4(0.0);
	
	//If the normal is facing the light
	if (NdotL > 0.0)
	{								 
		//Add the diffuse contribution
		color += vertDiffuse * NdotL;
		
		//Renormalize the half vector
		vec3 HV = normalize(halfVector);
		float NdotHV = max(dot(N, HV), 0.0);
		//Add the specular contribution
		color += material_specular * light0.specular * pow(NdotHV, material_shininess);	
	}
		//Calculate the attenuation factor
	float attenuation = 1.0 / (light0.constant_attenuation + 
                                 light0.linear_attenuation * dist + 
                                 light0.quadratic_attenuation * dist * dist);		

	//Output the final color after taking attenuation and ambient light into account								 	
	outColor = ((color * attenuation) + vertAmbient) * texture(texture0, texCoord0.st);	
}
