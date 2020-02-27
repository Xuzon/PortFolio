#version 130

precision highp float;


uniform sampler2D texture0;

in vec2 texCoord0;

out vec4 outColor;

void main(void) {
	outColor = texture(texture0, texCoord0.st);
	if (outColor.rgb == vec3(0.0, 0.0, 0.0)) {
		discard;
	}
}
