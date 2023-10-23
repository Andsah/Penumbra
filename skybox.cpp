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

void Skybox::Draw(const mat4 & viewMat, const mat4 & projMat) {

    texture->loadTextureToShader(shader);

    glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_TRUE, transformMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, GL_TRUE, viewMat.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projMatrix"), 1, GL_TRUE, projMat.m);

    
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    DrawModel(model, shader, "inPosition", NULL, "inTexCoord");
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}