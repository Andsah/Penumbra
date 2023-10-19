#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"
#include "headers/skybox.h"

Skybox::Skybox(std::string objFile, Texture * texture , GLuint shader, mat4 transformMatrix): 
    transformMatrix(transformMatrix),
    shader(shader),
    texture(texture)
    { model = LoadModel(objFile.c_str());}

Skybox::~Skybox() {
    DisposeModel(model);
}

Model* Skybox::getModel() {
    return model;
}

void Skybox::setTransform(mat4 newTransform) {
    transformMatrix = newTransform;
}

mat4 Skybox::getTransform() {
    return transformMatrix;
}

void Skybox::setShader(GLuint newShader) {
    shader = newShader;
}

void Skybox::Draw() {
    
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->loadTextureToShader(shader);
    
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_TRUE, transformMatrix.m);
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    DrawModel(model, shader, "inPosition", NULL, "inTexCoord");
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}