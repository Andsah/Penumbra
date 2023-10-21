#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"
#include "headers/gameObject.h"

#include <string>

GameObject::GameObject(std::string objFile, std::array<Texture *, NUM_TEX> textures, GLuint shader, mat4 transformMatrix): 
    transformMatrix(transformMatrix),
    shader(shader),
    textures(textures)
    { model = LoadModel(objFile.c_str());}

GameObject::GameObject(std::array<Texture *, NUM_TEX> textures, GLuint shader):
transformMatrix(T(0,0,0)),
textures(textures), 
shader(shader) {}

GameObject::~GameObject() {
    DisposeModel(model);
}

Model* GameObject::getModel() {
    return model;
}

void GameObject::setTransform(mat4 newTransform) {
    transformMatrix = newTransform;
}

void GameObject::setNormalMatrix(mat3 newNormalMat) {
    normalMatrix = newNormalMat;
}

mat4 GameObject::getTransform() {
    return transformMatrix;
}

void GameObject::setShader(GLuint newShader) {
    shader = newShader;
}

void GameObject::Draw() {
    for (uint i = 0; i < NUM_TEX; i++) {
        if (textures[i] != nullptr) {
            textures[i]->loadTextureToShader(shader);
        }
    }
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_TRUE, transformMatrix.m);
    glUniformMatrix3fv(glGetUniformLocation(shader, "normalMatrix"), 1, GL_TRUE, normalMatrix.m); // might not be needed anymore because of in-shader matrix
	DrawModel(model, shader, "inPosition", "inNormal", "inTexCoord", "inTangent", "inBitangent");
}
