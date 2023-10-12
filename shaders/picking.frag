#version 150

uniform uint gObjectIndex;
out vec3 out_Color;


void main(void) {
		out_Color = vec3(gObjectIndex, 0, 0);
}