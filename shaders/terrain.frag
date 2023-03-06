#version 150

in vec2 texCoord;
in vec3 normal;

uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec4 outColor;

void main(void)
{
	vec3 light = vec3(0, 10, 10);

	vec3 lightPos = mat3(viewMatrix * modelMatrix) * light;

	float shade;

	vec3 texNormal = normalize(texture(normalMap, texCoord).rgb);

	shade = dot(normalize(normal), normalize(lightPos)) * 3 - 1.5;
	shade = clamp(shade, 0, 1);

	outColor = shade * texture(textureMap, texCoord);
}
