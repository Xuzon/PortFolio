#version 120

uniform sampler2D texture0;
uniform sampler1D texture1;

varying vec4 color;
varying vec2 texCoord0;
varying float texCoord1;

void main(void) {
	gl_FragColor = color * texture2D(texture0, texCoord0.st) * texture1D(texture1, texCoord1);
}
