#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include "actor.h"
#include "camera.h"

// player, is an actor, has a camera
class Player: public Actor{
protected:


    // The position of the camera in world space
    vec3 playerPos {20.0f, 12.0f, 20.0f};
    // The position of the look-at point in world space
    vec3 lookAtPos {0.0f, 10.0f, 0.0f};
    // The initial up vector, normalized
    vec3 upVec {0.0f, 1.0f, 0.0f};
    // The distance one step should be when moving forward
    float moveDist {0.1f};

    //player collision (sphere)
    float radius = 1.0f;

    // Is the game paused?
    bool pause {true};
    // Check to make sure the game only switches pause-state once per keypress
    bool pPressed {false};

    struct leftMouseB {
        public:
        bool isPressed = false;
        bool pressCheck = false; 
        int x;
        int y;
    };

    leftMouseB leftButton;

    // toggle camera mode
    bool cPressed {false};
    bool flyingCamera{true};
    bool doneOncePerCamSwitch = false; // kinda dirty, think of better way.

    Camera * playerCam;

public:
    Player(float health);

    // Calculates the next look-at point based on the coordinates (x, y) of the of the mouse pointer
    void calcLookAt(int x, int y);
    // Function for handling inputs from the keyboard
    void checkKeyboardInput(Heightmap * hMap);
    // Function for handling mouse clicking, used for picking
    void mouseFunc(int button, int state, int x, int y);

    Camera * getPlayerCam();

    void updatePlayerCam();

    // shorthand for x of mouseclick, dont confuse with player position;
    int x();

    // shorthand again (see x)
    int y();

    // is leftbutton pressed?
    bool isPressed();

    void resetPressed();

};
#endif