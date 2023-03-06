#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/texture.h"

Texture::Texture(const char * diffuseFile, const char * normalFile, const char * specularFile) {
    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple(diffuseFile, &diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuse);

    glActiveTexture(GL_TEXTURE1);
    LoadTGATextureSimple(normalFile, &normalMap);
    glBindTexture(GL_TEXTURE_2D, normalMap);

    glActiveTexture(GL_TEXTURE2);
    LoadTGATextureSimple(specularFile, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    //printf("%i, %i, %i \n", diffuse, normalMap, specularMap);
}

void Texture::loadTextureToShader(GLuint shader) {
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "textureMap"), 0);

    glUniform1i(glGetUniformLocation(shader, "normalMap"), 1);

    glUniform1i(glGetUniformLocation(shader, "specularMap"), 2);
}