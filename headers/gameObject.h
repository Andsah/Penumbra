#ifndef OBJECT_H
#define OBJECT_H

#include <array>
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "texture.h"
#include "array2d.h"

#define NUM_TEX 8

// perhaps inherited classes with extra features are smart for gameplay purposes.
// A class for storing data about every object in the game
class GameObject
{
protected:

    // Contains the vertexArray, normalArray, texCoordArray, indexArray as well as some more things
    Model* model;  

    // An array of Texture objects
    std::array<Texture *,NUM_TEX>  textures;

    // The gameObject's translation, rotation, and scale combined, expressed in world coordinates
    mat4 transformMatrix;

    // Normal matrix to not distort normals
    mat3 normalMatrix;
    
    // Reference to the shader to be used during drawing
    GLuint shader;

    //More stuff? separate bump/normal map, specularity map?

public:
    // Take in the name of the .obj file, array of Texture objects, a shader program, and optionally a mat4 transformation matrix
    GameObject(const char * objFile, std::array<Texture *, NUM_TEX> textures, GLuint shader, mat4 transformMatrix = T(0,0,0));

    // Overload for terrain class's sake
    GameObject(std::array<Texture *, NUM_TEX> textures, GLuint shader);

    // Destructor
    ~GameObject();

    // Return a reference to the data Model object
    Model* getModel();

    // Set a new transformation to be the transformation of the gameObject
    void setTransform(mat4 newTransform);

    // Set a new normal matrix for the objects normals to be transformed by
    void setNormalMatrix(mat3 newNormalMat);

    // Return the transformationMatrix of the gameObject
    mat4 getTransform();

    // Change the shader to be used for this gameObject
    void setShader(GLuint newShader);

    // Draw the gameObject using the selected shader and transformation (mult with proj and world2view in shader)
    void Draw();
};
#endif