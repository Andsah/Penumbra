#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "gameObject.h"

GameObject::GameObject(const char objFile[], std::array<const char *,16> textureFiles, GLuint shader, mat4 transformMatrix): 
    transformMatrix(transformMatrix),
    shader(shader) 
    {
    meshData = LoadModel(objFile);
    for(unsigned i = 0; i < textureFiles.size(); i++) {
        LoadTGATextureSimple(textureFiles[i], &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}

GameObject::GameObject(GLuint shader):shader(shader) {}

GameObject::~GameObject() {
    DisposeModel(meshData);
}

Model* GameObject::getMeshData() {
    return meshData;
}

void GameObject::setTransform(mat4 newTransform) {
    transformMatrix = newTransform;
}

mat4 GameObject::getTransform() {
    return transformMatrix;
}

void GameObject::setShader(GLuint newShader) {
    shader = newShader;
}

void GameObject::Draw() {
    glUseProgram(shader); // why cant I get to these functions?
    //glUniform1i(glGetUniformLocation(shader, "tex"), 0); - decide on indexed textures or a set number plus space for normal maps etc.
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_TRUE, transformMatrix.m);
	DrawModel(meshData, shader, "inPosition", "inNormal", "inTexCoord");
}
