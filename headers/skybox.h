#ifndef SKYBOX_H
#define SKYBOX_H

#include <array>
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "texture.h"
#include "array2d.h"

// A class for representing a simple skybox that doesn't need all the things in a gameobject
class Skybox
{
protected:

    // Contains the vertexArray, normalArray, texCoordArray, indexArray as well as some more things
    Model* model;  

    // A Texture object for the skybox texture
    Texture * texture;

    // The skybox's translation, rotation, and scale combined, expressed in world coordinates, is cameraPos without rotation
    mat4 transformMatrix;
    
    // Reference to the shader to be used during drawing
    GLuint shader;

public:
    // Take in the name of the .obj file, array of Texture objects, a shader program, and optionally a mat4 transformation matrix
    Skybox(std::string objFile, Texture * texture, GLuint shader, mat4 transformMatrix = T(0,0,0));

    // Destructor
    ~Skybox();

    // Return a reference to the data Model object
    Model* getModel();

    // Set a new transformation to be the transformation of the gameObject
    void setTransform(mat4 newTransform);

    // Return the transformationMatrix of the skybox
    mat4 getTransform();

    // Change the shader to be used for this skybox
    void setShader(GLuint newShader);

    // Draw the skybox using the selected shader and transformation (mult with proj and world2view in shader) depth test and backface culling turned off
    void Draw();
};
#endif