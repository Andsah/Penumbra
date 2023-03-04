commondir = common/

# add camera.cpp to this? Yes says Christoffer
penumbra : penumbra.cpp camera.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c                                                                   # -lXt only works on lab pc
	g++ -Wall -o penumbra -I$(commondir) -I./common/Linux -DGL_GLEXT_PROTOTYPES penumbra.cpp camera.cpp $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

wipe : 
	rm penumbra