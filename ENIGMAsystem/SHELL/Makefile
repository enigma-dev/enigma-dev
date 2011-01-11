# Target `all` takes the following parameters (or defaults will be used):
#	GMODE{Run,Build,Debug,Compile}
#	AUDIO{<anything under audio_systems>}
#	COLLISION{<anything under collision_systems>}
#	GRAPHICS{<anything under graphics_systems>}
#	PLATFORM{<anything under platforms>}
#	GLINKS{<any linker args needed by any of the above>}
#	GFLAGS{<any additional flags to pass to g++>}
# The following parameter is also needed, but has no defaults:
#	OUTPUTNAME{<desired output name and directory>}

#apply default values
ifndef GMODE
	GMODE := Run
endif
ifndef GLINKS
	GLINKS := -lz
endif
ifndef AUDIO
	AUDIO := OpenAL
	GLINKS := $(GLINKS) -lopenal
endif
ifndef COLLISION
	COLLISION := BBox
endif
ifndef GRAPHICS
	GRAPHICS := OpenGL
	GLINKS := $(GLINKS) -lGL -lGLU
endif

#figure platform
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	ifndef PLATFORM
		PLATFORM := xlib
	endif
	CREMOVE := rm -f 
	ENDCREMOVE :=
	SLASHC := /
else ifeq ($(OS), Darwin)
	ifndef PLATFORM
		PLATFORM := Cocoa
	endif
	CREMOVE := rm -f 
	ENDCREMOVE :=
	SLASHC := /
else
	ifndef PLATFORM
		PLATFORM := Win32
	endif
	CREMOVE := del /Q '
	ENDCREMOVE :='
	SLASHC := \\
endif

default: all

needout:
ifndef OUTPUTNAME
# @ hides command echo
	@echo No OUTPUTNAME specified. Stopping.
	@exit 2
endif

all: needout build link

build:
	cd Graphics_Systems/$(GRAPHICS)/ 	&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="-static $(GFLAGS)"
	cd Platforms/$(PLATFORM)/ 			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="-static $(GFLAGS)"
	cd Universal_System/				&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="-static $(GFLAGS)"
	cd Audio_Systems/$(AUDIO)/			&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="-static $(GFLAGS)"
	cd Collision_Systems/$(COLLISION)/	&& $(MAKE) $(GMODE) MODE=$(GMODE) FLAGS="-static $(GFLAGS)"

link: needout
	g++ SHELLmain.cpp libEGMstd.cpp Graphics_Systems/$(GRAPHICS)/.eobjs_$(GMODE)/*.o Platforms/$(PLATFORM)/.eobjs_$(GMODE)/*.o Universal_System/.eobjs_$(GMODE)/*.o Audio_Systems/$(AUDIO)/.eobjs_$(GMODE)/*.o Collision_Systems/$(COLLISION)/.eobjs_$(GMODE)/*.o Audio_Systems/$(AUDIO)/.eobjs_$(GMODE)/*.a Audio_Systems/$(AUDIO)/.eobjs_$(GMODE)/libogg.a $(GLINKS) -o $(OUTPUTNAME)


clean:
	@echo $(OS)
	cd Graphics_Systems/OpenGL/ 	&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Graphics_Systems/OpenGLES/ 	&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Audio_Systems/OpenAL/ 		&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Platforms/Win32/ 			&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Platforms/xlib/ 				&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Universal_System/			&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
	cd Collision_Systems/none		&& $(MAKE) clean CREMOVE="$(CREMOVE)" SLASHC="$(SLASHC)" ENDCREMOVE="$(ENDCREMOVE)"
