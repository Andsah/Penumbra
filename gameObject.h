#ifndef OBJECT_H
#define OBJECT_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// A class for storing data about every object in the game, perhaps inherited classes with extra features are smart for gameplay purposes.
class gameObject
{
private:
    vec3 vertices; // should this option exist?
    Model* mesh;
    TextureData* texture;
    //More stuff? bump/normal map? array of normal vectors? 

public:
    gameObject(); // initialisation list for data members here

    ~gameObject();
};
#endif