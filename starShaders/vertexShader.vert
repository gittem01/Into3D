#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float brightness;

uniform mat4 pers;
uniform mat4 view;
uniform mat4 rotationMatrix; // Model

out float brightnessVG;

void main()
{
	vec4 r = pers * view * rotationMatrix * vec4(aPos, 1.0);
	gl_Position = r;
	brightnessVG = brightness;
}