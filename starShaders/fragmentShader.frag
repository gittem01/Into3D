#version 330 core

out vec4 FragColor;

in float brightnessGF;
in vec2 fragPos;

void main()
{
	float b = 1-sqrt(pow(fragPos[0], 2) + pow(fragPos[1], 2));
	if (b > 0.7)
		b = 1;
	FragColor = vec4(b, b, b, b);
	gl_FragDepth = 0.9999999;
}