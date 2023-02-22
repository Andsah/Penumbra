
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "camera.h"

void Camera::calcLookAt(int x, int y) {
    if (!pause) {
        vec3 forward = normalize(lookAtPos - cameraPos);
        vec3 rightVec = normalize(CrossProduct(forward, upVec));
        vec3 trueUp = normalize(CrossProduct(forward, rightVec));
        lookAtPos = cameraPos + normalize(forward + ((x - 400) * rightVec + (y - 400) * trueUp)*cameraSensitivity); 
        // find way to let screen dimensions be arbitrary
        glutWarpPointer(300, 300);
    }
}

void Camera::checkKeyboardInput() {
    vec3 forward = normalize(lookAtPos - cameraPos);
    vec3 rightVec = normalize(CrossProduct(forward, upVec)); // Idk if I should update this to trueUp or limit the camera viewing angles 
    vec3 trueUp = normalize(CrossProduct(forward, rightVec));
    if (glutKeyIsDown('w')) { 
        cameraPos = cameraPos + forward * moveDist; 
        // to make player a fixed height off ground, 
        // either project forward onto the terrain slope, 
        // or maybe force a fixed dist between camera and ground node underneath
        lookAtPos = lookAtPos + forward * moveDist;
    }
    if (glutKeyIsDown('a')) {
        cameraPos = cameraPos - rightVec * moveDist;
        lookAtPos = lookAtPos - rightVec * moveDist;
    }
    if (glutKeyIsDown('s')) {
        cameraPos = cameraPos - forward * moveDist;
        lookAtPos = lookAtPos - forward * moveDist;
    }
    if (glutKeyIsDown('d')) {
        cameraPos = cameraPos + rightVec * moveDist;
        lookAtPos = lookAtPos + rightVec * moveDist;
    }
    if (glutKeyIsDown('z')) {
        cameraPos = cameraPos + trueUp * moveDist;
        lookAtPos = lookAtPos + trueUp * moveDist;
    }
    if (glutKeyIsDown('c')) {
        cameraPos = cameraPos - trueUp * moveDist;
        lookAtPos = lookAtPos - trueUp * moveDist;
    }
    if (glutKeyIsDown('e')) {
        exit(0);
    }
    if (glutKeyIsDown('p')) {
        if (!pPressed) {
            pPressed = true;
            pause = !pause;
            if (pause) {glutShowCursor();}
            else {glutHideCursor();}
        }
    } 
    else {
        pPressed = false;
    }
}