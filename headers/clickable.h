#ifndef CLICK_H
#define CLICK_H

#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include "gameObject.h"
#include <functional>

//typedef void (*callbackFunc)(void);

#define callbackFunc std::function<void()>

// A class for representing a portal in the game
class Clickable: public GameObject{ 

    protected:
        // the callback function that is called when a user clicks the clickable object
        std::function<void()> callback;
    
    public:

        // Going to need a constructor
        Clickable(std::string objFile, std::array<Texture *,NUM_TEX> textureFiles, GLuint shader,  mat4 transformMatrix = T(0,0,0));

        void onClick();

        void setOnCLick(callbackFunc newCallback);

};

#endif