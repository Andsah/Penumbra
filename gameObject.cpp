#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "headers/gameObject.h"

GameObject::GameObject(const char objFile[], std::array<const char *, NUM_TEX> textureFiles, GLuint shader, const char * normalMapFile, mat4 transformMatrix): 
    transformMatrix(transformMatrix),
    shader(shader)
    {
    model = LoadModel(objFile);
    if (normalMapFile != "") {LoadTGATextureSimple(normalMapFile, &normalMap);} // for detailed bumps on the texture
    for(unsigned i = 0; i < textureFiles.size(); i++) {
        LoadTGATextureSimple(textureFiles[i], &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}

GameObject::GameObject(GLuint shader):shader(shader) {}

GameObject::~GameObject() {
    DisposeModel(model);
}

Model* GameObject::getModel() {
    return model;
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
	DrawModel(model, shader, "inPosition", "inNormal", "inTexCoord");
}
