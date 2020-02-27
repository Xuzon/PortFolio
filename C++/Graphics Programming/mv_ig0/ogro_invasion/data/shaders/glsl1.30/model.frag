#version 130

precision highp float;


uniform sampler2D texture0;

in vec2 texCoord0;

out vec4 outColor;

void main(void) {
	//Sample the texture
	outColor = texture(texture0, texCoord0.st);	
}
