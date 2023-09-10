#version 150

in vec2 texCoords;
out vec4 out_Color;

uniform sampler2D textureMap;

void main(void) {
		out_Color = texture(textureMap, texCoords);
}
