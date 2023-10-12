#version 150

uniform float objectIndex;
uniform float numObjects;
out vec3 out_Color;


void main(void) {
		out_Color = vec3(objectIndex/numObjects, 0, 0);
}