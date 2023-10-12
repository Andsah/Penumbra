#ifndef PICKING_H
#define PICKING_H

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

class PickingTex {

    public:

    FBOstruct * fbo; // m_fbo is fb, m_pickingtexture is texid and depthtexture is depth

    //public:
    
    PickingTex();

    void init(uint width, uint height);

    ~PickingTex();

    void enableWriting();

    void disableWriting();

    struct PixelInfo
    {
        uint objectID; 
        uint junk1; 
        uint junk2;
        uint junk3; 

        void Print()
        {
            printf("Object %d\n", objectID);
        }
    };

    PixelInfo readPixel(uint x, uint y);
    
};

#endif