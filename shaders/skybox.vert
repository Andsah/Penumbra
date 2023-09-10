#version 150

in vec3 inPosition;
in vec2 inTexCoord;

uniform mat4 projMatrix;
uniform mat4 modelMatrix; // revise these variable names in shaders
uniform mat4 viewMatrix;

out vec2 texCoords;

void main(void) {
	texCoords = inTexCoord;
	vec4 v = modelMatrix * vec4(inPosition, 1.0);
	gl_Position = projMatrix * viewMatrix * v;
}
