#version 330 core 

// these come from the DrawModel() call
in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
in  vec3 inTangent;
in  vec3 inBitangent;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 surfaceNormal;
out vec2 texCoord;
out vec4 surfacePos;
out mat3 TBNMatrix;

out float height;

void main(void) {
	surfaceNormal = mat3(transpose(inverse(viewMatrix * modelMatrix))) * inNormal;
	texCoord = inTexCoord;
	surfacePos = viewMatrix * modelMatrix * vec4(inPosition, 1.0);

	// create TBN matrix for snazzy normal maps!
	vec3 T = normalize(vec3(modelMatrix * vec4(inTangent,   0.0)));
    vec3 B = normalize(vec3(modelMatrix * vec4(inBitangent, 0.0)));
    vec3 N = normalize(vec3(modelMatrix * vec4(inNormal,    0.0)));
    TBNMatrix = mat3(T, B, N);

	height = (modelMatrix * vec4(inPosition, 1.0)).y /32;

	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
