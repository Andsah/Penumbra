#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/texture.h"
#include <string>

Texture::Texture(std::string diffuseFile, std::string normalFile, std::string specularFile) {
    LoadTGATextureSimple(diffuseFile.c_str(), &diffuse);

    if (!normalFile.empty()) {
    LoadTGATextureSimple(normalFile.c_str(), &normalMap);
    }
    else {normalMap = NULL;}

    if (specularFile.empty()) {
    LoadTGATextureSimple(specularFile.c_str(), &specularMap);
    }
    else {specularMap = NULL;}

    //printf("%i, %i, %i \n", diffuse, normalMap, specularMap);
}

void Texture::loadTextureToShader(GLuint shader) {
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    glUniform1i(glGetUniformLocation(shader, "textureMap"), 0);

    if (normalMap != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
    }

    if (specularMap != NULL) {
        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);
        glBindTexture(GL_TEXTURE_2D, specularMap);
    }
}