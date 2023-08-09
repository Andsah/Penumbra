#version 330 core 

// these come from the DrawModel() call
in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
// in  vec3 intangent;
// in  vec3 inBiTangent;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 surfaceNormal;
out vec2 texCoord;
out vec4 surfacePos;

void main(void) {
	surfaceNormal = mat3(transpose(inverse(viewMatrix * modelMatrix))) * inNormal;
	texCoord = inTexCoord;
	surfacePos = viewMatrix * modelMatrix * vec4(inPosition, 1.0);

	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
