#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/light.h"

Light::Light(vec3 diffuse, vec3 ambient, vec3 qualia, bool is_positional, vec3 specular): 
diffuse(diffuse), 
ambient(ambient),
qualia(qualia), 
is_positional(is_positional),
specular(specular){};