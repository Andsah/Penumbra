#ifndef OBJECT_H
#define OBJECT_H

#include <array>
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// perhaps inherited classes with extra features are smart for gameplay purposes.
// A class for storing data about every object in the game
class GameObject
{
protected:

    // Contains the vertexArray, normalArray, texCoordArray, indexArray as well as some more things
    Model* meshData;

    // An array of 16 GLuints that each point to a loaded TGA texture
    std::array<GLuint,16>  textures;

    // The gameObject's translation, rotation, and scale combined, expressed in world coordinates
    mat4 transformMatrix;
    
    // Reference to the shader to be used during drawing
    GLuint shader;

    //More stuff? separate bump/normal map, specularity map?

public:
    // Take in the name of the .obj file, array of names of .tga files, a shader program, and optionally a mat4 transformation matrix
    GameObject(const char objFile[], std::array<const char *,16> texturesFiles, GLuint shader, mat4 transformMatrix = T(0,0,0));

    // Overload for terrain class's sake
    GameObject(GLuint shader);

    // Destructor
    ~GameObject();

    // Return a reference to the data Model object
    Model* getMeshData();

    // Set a new transformation to be the transformation of the gameObject
    void setTransform(mat4 newTransform);

    // Return the transformationMatrix of the gameObject
    mat4 getTransform();

    // Change the shader to be used for this gameObject
    void setShader(GLuint newShader);

    // Draw the gameObject using the selected shader and transformation (mult with proj and world2view in shader)
    void Draw();
};
#endif