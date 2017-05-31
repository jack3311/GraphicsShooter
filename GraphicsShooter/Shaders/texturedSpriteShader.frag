#version 430 core

in vec3 outColor;
in vec2 outTexCoord;

out vec4 color;

uniform sampler2D texture;

void main()
{
	vec4 texColor = texture2D(texture, outTexCoord);

	if (texColor.a < 0.1f) discard;

    color = texColor * vec4(outColor, 1.0f);
	//color = vec4(outColor, 1.0f);
}