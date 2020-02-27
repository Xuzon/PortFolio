#version 130

precision highp float;



uniform sampler2D texture0;

in vec4 color;
in vec2 texCoord0;

out vec4 outColor;

void main(void) {
	outColor = color * texture(texture0, texCoord0.st);	
}
