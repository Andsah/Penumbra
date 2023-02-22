commondir = common/

# add controller.cpp to this? Yes says Christoffer
penumbra : penumbra.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o penumbra -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES penumbra.cpp controller.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

wipe : 
	rm penumbra