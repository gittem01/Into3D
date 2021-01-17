#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 pers;
uniform mat4 view;
uniform mat4 rot;

out vec2 texCoords;
out vec3 theNormal;
out vec3 fragPos;

void main()
{
	gl_Position = pers * view * model * vec4(aPos, 1.0);
	texCoords = texCoord;
	theNormal = vec3(rot * vec4(normal, 1));
	fragPos = vec3(model * vec4(aPos, 1.0f));
}