#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils4.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

#include "headers/pickingTex.h"


PickingTex::PickingTex() {
}

void PickingTex::init(uint width, uint height) {
    fbo = initFBO2(width, height, 0, 1);
}


PickingTex::~PickingTex() {
}

void PickingTex::enableWriting() {
    useFBO(fbo, 0L, 0L);
}

void PickingTex::disableWriting() {
    useFBO(0L, 0L, 0L);
}

PickingTex::PixelInfo PickingTex::readPixel(uint x, uint y)
{

    //useFBO(0L, fbo, 0L);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->fb);
    printf("GLgetError1: %u\n", glGetError());
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    printf("GLgetError2: %u\n", glGetError());

    PixelInfo pixel;
    glReadPixels(x, y, 1, 1, GL_RGBA8,  GL_UNSIGNED_BYTE, &pixel); // not reading the info correctly
    printf("GLgetError3: %u\n", glGetError());


    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0L);
    pixel.Print();
    printf("--------------\n");

    return pixel;
}
