#version 430 core

in vec3 outNormal;
in vec3 outFragPos;
in vec2 outTexCoord;

uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];
uniform vec3 camPos;
uniform float ambientStrength;
uniform vec3 ambientCol;
uniform float specularStrength;
uniform int numLights;

out vec4 color;

uniform sampler2D texture_0_0;

void main()
{
	vec3 norm = normalize(outNormal);
	vec3 viewDir = normalize(camPos - outFragPos);


	vec3 lightSum = vec3(0.f, 0.f, 0.f);
	

	//Ambient
	vec3 ambient = ambientStrength * ambientCol;
	lightSum += ambient;


	for (int l = 0; l < numLights; l++)
	{
		//For each light:
		vec3 lightDir = normalize(lightPositions[l] - outFragPos);

	
		//Diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColors[l];

		//Specular
		vec3 reflectionDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
		vec3 specular = spec * lightColors[l] * specularStrength;


		//Add to summed lighting value
		lightSum += diffuse;
		lightSum += specular;
	}


	vec4 texColor = texture2D(texture_0_0, outTexCoord);

	//color = vec4(1.0, 1.0, 1.0, 1.0) * vec4(lightSum, 1.f);
	color = texColor * vec4(lightSum, 1.f);
}