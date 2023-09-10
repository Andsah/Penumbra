#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "headers/gameObject.h"

GameObject::GameObject(const char * objFile, std::array<Texture *, NUM_TEX> textures, GLuint shader, mat4 transformMatrix): 
    transformMatrix(transformMatrix),
    shader(shader),
    textures(textures)
    { model = LoadModel(objFile);}

GameObject::GameObject(std::array<Texture *, NUM_TEX> textures, GLuint shader):
transformMatrix(S(1)),
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
