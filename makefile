commondir = common/

gamefiles = penumbra.cpp camera.cpp gameObject.cpp terrain.cpp light.cpp texture.cpp skybox.cpp clickable.cpp heightmap.cpp billboard.cpp actor.cpp player.cpp portal.cpp

penumbra : $(gamefiles) $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c                                                                   # -lXt only works on lab pc
	g++ -Wall -o penumbra -I$(commondir) -I./common/Linux -DGL_GLEXT_PROTOTYPES $(gamefiles) $(commondir)GL_utilities.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

wipe : 
	rm penumbra