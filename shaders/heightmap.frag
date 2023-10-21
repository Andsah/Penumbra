#version 150

out vec4 out_Color;

in float height;


void main(void) {
		out_Color = vec4(height, height, height, 1.0);
}