#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 outNormal;
out vec2 outTexCoord;
out vec3 outFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void)
{
	outNormal = mat3(transpose(inverse(model))) * normal;
	outTexCoord = texCoord;
	outFragPos = vec3(model * vec4(position, 1.f));
	gl_Position = projection * view * model * vec4(position, 1.0f);
}