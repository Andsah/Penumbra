#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/texture.h"

Texture::Texture(const char * diffuseFile, const char * normalFile, const char * specularFile) {
    LoadTGATextureSimple(diffuseFile, &diffuse);

    if (normalFile != NULL) {
    LoadTGATextureSimple(normalFile, &normalMap);
    }

    if (specularFile != NULL) {
    LoadTGATextureSimple(specularFile, &specularMap);
    }

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