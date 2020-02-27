#version 120

uniform sampler2D texture0;

varying vec2 texCoord0;

void main(void) {
	//Sample the texture
	vec4 outColor = texture2D(texture0, texCoord0.st);	

	//If the alpha component is too low then discard
	//this fragment
	if (outColor.a < 0.1) {
		discard;
	}
	
	gl_FragColor = outColor;
}
