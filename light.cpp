#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/light.h"
#include <string>

Light::Light(vec3 diffuse, vec3 ambient, vec3 qualia, bool is_positional, vec3 specular): 
diffuse(diffuse), 
ambient(ambient),
qualia(qualia), 
is_positional(is_positional),
specular(specular){};

void Light::uploadLight(GLuint shader, int index) {
    std::string pos = std::string("lightPos[") + std::to_string(index) + std::string("]"); // so we get lightPos[1], lightPos[2]...
		glUniform3fv(glGetUniformLocation(shader, pos.c_str()), 1, &qualia.x);

    std::string amb = std::string("lightAmb[") + std::to_string(index) + std::string("]");
		glUniform3fv(glGetUniformLocation(shader, amb.c_str()), 1, &ambient.x);

    std::string diff = std::string("lightDiff[") + std::to_string(index) + std::string("]");
		glUniform3fv(glGetUniformLocation(shader, diff.c_str()), 1, &diffuse.x);

    std::string spec = std::string("lightSpec[") + std::to_string(index) + std::string("]");
		glUniform3fv(glGetUniformLocation(shader, spec.c_str()), 1, &specular.x);

    std::string isPos = std::string("isPos[") + std::to_string(index) + std::string("]");
		glUniform1i(glGetUniformLocation(shader, isPos.c_str()), is_positional);
};