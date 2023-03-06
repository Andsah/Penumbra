#version 150

// these come from the DrawModel() call
in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;

out vec2 texCoord;
out vec3 normal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void) {
	mat3 normalMatrix = mat3(viewMatrix * modelMatrix);
	normal = normalMatrix * normalize(inNormal);
	texCoord = inTexCoord;
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
