# Target `all` takes the following parameters:
#	GMODE{run,build,debug,compile}
#	GRAPHICS{<anything under graphics_systems>}
#	PLATFORM{<anything under platforms>}
#	GLINKS{<anything needed by the above as linker args>}
all: build link

build:	
	cd Graphics_Systems/$(GRAPHICS)/ 	&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Platforms/$(PLATFORM)/ 			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Universal_System/				&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Audio_Systems/OpenAL/			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"
	cd Collision_Systems/none/			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="$(GFLAGS)"

link:
	g++ SHELLmain.cpp libEGMstd.cpp Graphics_Systems/$(GRAPHICS)/.eobjs_$(GMODE)/*.o Platforms/$(PLATFORM)/.eobjs_$(GMODE)/*.o Universal_System/.eobjs_$(GMODE)/*.o Audio_Systems/OpenAL/.eobjs_$(GMODE)/*.o Collision_Systems/none/.eobjs_$(GMODE)/*.o Audio_Systems/OpenAL/.eobjs_$(GMODE)/*.a Audio_Systems/OpenAL/.eobjs_$(GMODE)/libogg.a $(GLINKS) -o $(OUTPUTNAME)

# Figure platform
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	CREMOVE := rm -f 
	ENDCREMOVE :=
	SLASHC := /
else ifeq ($(OS), Darwin)
	CREMOVE := rm -f 
	ENDCREMOVE :=
	SLASHC := /
else
	CREMOVE := del /Q '
	ENDCREMOVE :='
	SLASHC := \\
endif

clean:
	echo $(OS)
	cd Graphics_Systems/OpenGL/ 	&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Graphics_Systems/OpenGLES/ 	&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Audio_Systems/OpenAL/ 		&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Platforms/Win32/ 			&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Platforms/xlib/ 				&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Universal_System/			&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Collision_Systems/none		&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
