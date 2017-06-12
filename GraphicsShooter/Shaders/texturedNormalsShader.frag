#version 430 core

in vec3 outNormal;
in vec3 outFragPos;
in vec2 outTexCoord;

uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];
uniform vec3 lightSpotDirs[10];
uniform float lightSpotAngles[10];

uniform vec3 camPos;
uniform float ambientStrength;
uniform vec3 ambientCol;
uniform float specularStrength;
uniform int numLights;
uniform float reflectionStrength;

out vec4 color;

uniform samplerCube skybox;
uniform sampler2D texture_0_0;

void main()
{
	vec3 norm = normalize(outNormal);
	vec3 viewDir = normalize(camPos - outFragPos);

	vec3 lightSum = vec3(0.f, 0.f, 0.f);
	
	//Ambient
	vec3 ambient = ambientStrength * ambientCol;
	lightSum += ambient;

	//Other factors
	for (int l = 0; l < numLights; l++)
	{
		//For each light:
		vec3 lightDir = normalize(lightPositions[l] - outFragPos);

		//Check for spotlight
		if (lightSpotAngles[l] > 0)
		{
			float lightToSurfaceFromDirAngle = acos(dot(lightDir, lightSpotDirs[l]));
			if (lightToSurfaceFromDirAngle > lightSpotAngles[l]) //If this fragment is not lit
				continue;
		}


	
		//Diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColors[l];

		//Specular
		vec3 reflectionDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
		vec3 specular = spec * lightColors[l] * specularStrength;

		//Rim
		float rimFactor = 1.0 - dot(norm, viewDir);
		rimFactor = smoothstep(0.0, 1.0, rimFactor);
		rimFactor = pow(rimFactor, 16);
		vec3 rim = rimFactor * vec3(1.0f, 0.6f, 0.2f) * lightColors[l];


		//Add to summed lighting value
		lightSum += diffuse;
		lightSum += specular;
		lightSum += rim;
	}

	//Reflections:
	vec3 reflectDir = reflect(-viewDir, norm);
	vec4 skyboxReflectColor = texture(skybox, reflectDir);
	

	vec4 texColor = texture2D(texture_0_0, outTexCoord);
	color = texColor * vec4(lightSum, 1.f) + reflectionStrength * skyboxReflectColor;
}