#version 120

uniform sampler2D texture0;

varying vec2 texCoord0;

void main(void) {
	//Sample the texture
	gl_FragColor = texture2D(texture0, texCoord0.st);	
}
