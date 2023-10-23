#ifndef TEXTURE_H
#define TEXTURE_H

#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include <string>

// Contains diffuse, normal and specular maps and assists in loading them to the shader
struct Texture {

    // The texture itself, most important
    GLuint diffuse;

    // The normal map gives added shadow depth to a texture
    GLuint normalMap;

    // Defines what areas are more or less shiny
    GLuint specularMap;

    // Metalness map, RoughnessMap, HeightMap, AOMap - sound a bit too ambitious and low yield.

    Texture(std::string diffuseFile, std::string normalFile, std::string specularFile);

    // Takes a shader and loads the diffuse, normal and specular maps into the shader 
    // [CALLS glUseProgram FOR YOU]
    void loadTextureToShader(GLuint shader);
};

#endif