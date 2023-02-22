#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

struct Controller {

    Controller(vec3* cameraPos, vec3* lookAtPos, vec3* upVec): 
     cameraPos {cameraPos}, lookAtPos {lookAtPos}, upVec {upVec} {}

    vec3* cameraPos;
    vec3* lookAtPos;
    vec3* upVec;
    float moveDist {1.0f};
    float cameraSensitivity {0.0005f};
    bool pause {true};
    bool pPressed {false};

    void calcLookAt(int x, int y);

    void checkKeyboardInput();
}

#endif