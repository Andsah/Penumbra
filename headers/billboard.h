#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <vector>
#include "../common/Linux/MicroGlut.h"
#include "../common/GL_utilities.h"
#include "../common/VectorUtils4.h"
#include "../common/LittleOBJLoader.h"
#include "../common/LoadTGA.h"

#include "texture.h"
#include "heightmap.h"


class Billboard
{
protected:

    std::vector<vec3> startVerts;  

    Texture *  texture;

    Heightmap * hMap;
    
    // Reference to the shader to be used during drawing
    GLuint shader;

    GLuint vertexArrayObjID;
    GLuint vertexBufferObjID;

public:
    Billboard(Texture * texture, GLuint shader);

    void generateVerts(uint num);

    // Change the shader to be used for this gameObject
    void setShader(GLuint newShader);

    void setHeightmap(Heightmap * newMap);

    // Simple draw for the points only
    void Draw(const mat4 & viewMat, const mat4 & projMat);
};
#endif