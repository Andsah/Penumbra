#ifndef LIGHT_H
#define LIGHT_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "gameObject.h"

// Respresents a lightsource in the game. Can be positional or directional
struct Light {

    // The colour of the light
    vec3 colour;

    // Describes either the position or the direction of the light, depending on if it is positional or directional
    vec3 qualia;

    // Is the light at a specific location or shining in a specific direction?
    bool is_positional;

    // Let there be light
    Light(vec3 colour, vec3 qualia, bool is_positional);
};

#endif