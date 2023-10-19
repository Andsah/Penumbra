#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils4.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"

#include "headers/clickable.h"


Clickable::Clickable(std::string objFile, std::array<Texture *, NUM_TEX> textureFiles, GLuint shader, mat4 transformMatrix):
GameObject(objFile, textureFiles, shader, transformMatrix) {}

void Clickable::onClick() {
    callback();
}

void Clickable::setOnCLick(callbackFunc newCallback) {
    callback = newCallback;
}
