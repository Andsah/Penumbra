#version 150

in vec3 inPosition;

uniform mat4 projMatrix; // don't think I should use projection on the heightmap
uniform mat4 modelMatrix; // revise these variable names in shaders
uniform mat4 viewMatrix;

out float height;

void main(void) {
	vec4 v = modelMatrix * vec4(inPosition, 1.0);
	gl_Position = projMatrix * viewMatrix * v;

    height = (modelMatrix * vec4(inPosition, 1.0)).y / 12.75;
}