#ifndef CAMERA_H
#define CAMERA_H

// window width
#define WIN_W 800
// window height
#define WIN_H 800

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// Struct for handling variables and functions for controlling the players movement and camera
struct Camera {
    
    // The position of the camera in world space
    vec3 cameraPos {20.0f, 12.0f, 20.0f};
    // The position of the look-at point in world space
    vec3 lookAtPos {0.0f, 10.0f, 0.0f};
    // The initial up vector, normalized
    vec3 upVec {0.0f, 1.0f, 0.0f};
    // The distance one step should be when moving forward
    float moveDist {1.0f};
    // The sensitivity of the mouse-controlled camera rotation
    float cameraSensitivity {0.0005f};
    // Is the game paused?
    bool pause {true};
    // Check to make sure the game only switches pause-state once per keypress
    bool pPressed {false};

    // Calculates the next look-at point based on the coordinates (x, y) of the of the mouse pointer
    void calcLookAt(int x, int y);
    // Function for handling inputs from the keyboard
    void checkKeyboardInput();
};
#endif