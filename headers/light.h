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

    // The diffuse colour of the light
    vec3 diffuse;

    // The colour of the specular portion of light, often just vec3(1.0f)
    vec3 specular;
    
    // The ambient light generated from this light, usually a low value
    vec3 ambient;

    // Describes either the position or the direction of the light, depending on if it is positional or directional
    vec3 qualia;

    // Is the light at a specific location or shining in a specific direction?
    bool is_positional;

    // Let there be light
    Light(vec3 diffuse, vec3 ambient, vec3 qualia, bool is_positional, vec3 specular = vec3(1.0f));
};

#endif