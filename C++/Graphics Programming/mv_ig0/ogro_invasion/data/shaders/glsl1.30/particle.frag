#version 130

precision highp float;


uniform sampler2D texture0;

in vec4 color;

void main(void) {
	//Sample the texture
	gl_FragColor = texture(texture0, gl_PointCoord) * color;
}
