#include "headers/camera.h"
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// collision detection will probably have to be done in here

void Camera::calcLookAt(int x, int y) {
    if (!pause) {
        vec3 forward = normalize(lookAtPos - cameraPos);
        vec3 rightVec = normalize(CrossProduct(forward, upVec));
        vec3 trueUp = normalize(CrossProduct(forward, rightVec));
        lookAtPos = cameraPos + normalize(forward + ((x - WIN_W/2) * rightVec + (y - WIN_H/2) * trueUp)*cameraSensitivity); 
        // find way to let screen dimensions be arbitrary
        // and why its not working on my windows/ubuntu setup - or thinlinc
        glutWarpPointer(WIN_W/2, WIN_H/2);
    }
}

void Camera::checkKeyboardInput() {
    vec3 forward = normalize(lookAtPos - cameraPos);
    vec3 rightVec = normalize(CrossProduct(forward, upVec)); 
    // Idk if I should update this to trueUp or limit the camera viewing angles // NO that was not it lol
    if (glutKeyIsDown('w')) { 
        cameraPos = cameraPos + forward * moveDist; 
        // to make player a fixed height off ground, 
        // either project forward (and the others) onto the terrain slope (tangent), 
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