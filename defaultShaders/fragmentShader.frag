#version 330 core

out vec4 FragColor;

in vec2 texCoordsF;
in vec3 theNormalF;
in vec3 fragPosF;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objColor;

void main()
{
	vec3 lightPos = vec3(0, 12, 0);
	vec3 lightDir = lightPos - fragPosF;
	float dist = length(lightDir)*0.1;
	float diff = max(dot(theNormalF, normalize(lightDir)), 0.0);
	vec3 diffuse = diff * lightColor * objColor;
	vec3 ambient = 1 * lightColor * objColor;
	vec3 result = diffuse + ambient;
	FragColor = vec4(result/pow(dist, 2), 1.0);
}