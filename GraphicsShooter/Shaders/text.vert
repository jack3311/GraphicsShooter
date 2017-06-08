#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projection;

void main() {
	//Use 1.0 z value to fix depth test
	gl_Position = projection * vec4(vertex.xy, 1.0, 1.0);
	TexCoords = vertex.zw;
}