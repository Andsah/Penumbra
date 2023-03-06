#ifndef TEXTURE_H
#define TEXTURE_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// Contains diffuse, normal and specular maps and assists in loading them to the shader
struct Texture {

    // The texture itself, most important
    GLuint diffuse;

    // The normal map gives added shadow depth to a texture
    GLuint normalMap;

    // Defines what areas are more or less shiny
    GLuint specularMap;

    // Metalness map, RoughnessMap, HeightMap, AOMap - sound a bit too ambitious and low yield.

    Texture(const char * diffuseFile, const char * normalFile, const char * specularFile);

    // Takes a shader and loads the diffuse, normal and specular maps into the shader
    void loadTextureToShader(GLuint shader);
};

#endif