#version 130

precision highp float;

uniform sampler2D texture0;
uniform sampler1D texture1;

in vec4 color;
in vec2 texCoord0;
in float texCoord1;
out vec4 outColor;

void main(void) {
	outColor = color * texture(texture0, texCoord0.st) * texture(texture1, texCoord1);	// 
}
