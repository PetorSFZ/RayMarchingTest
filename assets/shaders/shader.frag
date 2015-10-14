#version 330

precision highp float; // required by GLSL spec Sect 4.5.3

// Input
in vec2 uvCoord;

void main()
{
	gl_FragColor = vec4(0.2, 0.5, 0.5, 1);
}