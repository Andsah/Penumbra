#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "gameObject.h"

#include "light.h"

Light::Light(vec3 colour, vec3 qualia, bool is_positional): 
colour(colour), 
qualia(qualia), 
is_positional(is_positional){};