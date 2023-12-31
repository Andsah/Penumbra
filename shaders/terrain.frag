#version 330 core

in vec2 texCoord;
in vec4 surfacePos;
in vec3 surfaceNormal;
in mat3 TBNMatrix;

in float height;

uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

uniform vec3 lightPos[10];
uniform vec3 lightAmb[10]; 
uniform vec3 lightDiff[10]; 
uniform vec3 lightSpec[10]; 
uniform bool isPos[10]; 
uniform float specularity[10]; 

out vec4 outColor;

void main(void)
{	
	float amb = 1.0;
	vec3 ambient = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	for(int i = 0; i < 10; i++){
	// ambient
	ambient += amb * lightAmb[i];

	if(isPos[i]){
	// diffuse
	vec3 lightPos = vec3(viewMatrix * vec4(lightPos[i], 1.0));
	vec3 s = normalize(lightPos - vec3(surfacePos));
	vec3 n = normalize(surfaceNormal + 10 * mat3(transpose(inverse(viewMatrix * modelMatrix))) * TBNMatrix * texture(normalMap, texCoord).rgb);
	float diff = clamp(dot(n, s), 0, 1);
	float d = length(lightPos - vec3(surfacePos));
	float attenuation = min((1.0/( 0.001 * d * d)), 1.0);
	diffuse += attenuation * diff * lightDiff[i];

	// specular
	vec3 s_vertical = dot(-s, n) * n;
	vec3 s_horizontal = -s - s_vertical;
	vec3 r = -s_horizontal + s_vertical;
	float spec = pow(clamp(dot(r, normalize(vec3(surfacePos) /*+ mat3(transpose(inverse(viewMatrix * modelMatrix))) * TBNMatrix * texture(specularMap, texCoord).rgb*/)),0, 1), 100);
	specular += attenuation * attenuation * spec * lightSpec[i];

	}
	else { // TODO: add back in directional lights, probably just a static sun tho
		// diffuse

		// specular
	}
	}
	// combine
	outColor = vec4((ambient + diffuse + specular) * texture(textureMap, texCoord).rgb, 1.0);

	//outColor = vec4(height, height, height, 1.0);
}
