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
#include "terrain.h"

// Struct for handling variables and functions for controlling the players movement and camera
struct Camera {
    
    // Matrix for projecting the view onto the screen(?)
    mat4 projectionMatrix;
    // Matrix for translating the worldspace into view-coordinates
    mat4 world2viewMatrix;
    // The position of the camera in world space
    vec3 cameraPos {20.0f, 12.0f, 20.0f};
    // The position of the look-at point in world space
    vec3 lookAtPos {0.0f, 10.0f, 0.0f};
    // The initial up vector, normalized
    vec3 upVec {0.0f, 1.0f, 0.0f};
    // The distance one step should be when moving forward
    float moveDist {0.1f};
    // The sensitivity of the mouse-controlled camera rotation
    float cameraSensitivity {0.0005f};
    // Is the game paused?
    bool pause {true};
    // Check to make sure the game only switches pause-state once per keypress
    bool pPressed {false};

    // toggle camera mode
    bool cPressed {false};
    bool flyingCamera{true};
    bool doneOncePerCamSwitch = false; // kinda dirty, think of better way.

    // rework camera to use pitch and yaw and be limited vertically. also the whole walk on the plane thing
    // maybe raycast straight down and pass object and x,y coords to calc height?

    // Set ProjectionMatrix
    void setProjMat(mat4 projMat);
    // Update the world2view matrix (done every tick)
    void updateWorld2view();
    // Calculates the next look-at point based on the coordinates (x, y) of the of the mouse pointer
    void calcLookAt(int x, int y);
    // Function for handling inputs from the keyboard
    void checkKeyboardInput(Terrain * terrain);
    // Function for handling mouse clicking, used for picking
    void mouseFunc(int button, int state, int x, int y);
    // Calculating the appropriate player height on the terrain map - how to expand this to placed floors etc? kanske designa egen terrainmap med platta områden som golv
    float calcHeight(Model * terrain, int terrainWidth, float x, float z);
};
#endif