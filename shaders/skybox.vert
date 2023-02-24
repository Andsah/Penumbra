#version 150

in vec3 in_Position; // Look in lab 3 to remember what this was
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix; // revise these variable names in shaders
uniform mat4 lookAtMatrix;

out vec2 texCoords;

void main(void) {
	texCoords = inTexCoord;
	vec4 v = mdlMatrix * vec4(in_Position, 1.0);
	gl_Position = projectionMatrix * lookAtMatrix * v;
}
