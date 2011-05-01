
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
else
	ifeq ($(OS), Darwin)
		ifndef PLATFORM
			PLATFORM := Cocoa
		endif
	else
		ifndef PLATFORM 
			PLATFORM := Win32
		endif
	endif
endif

default: game
all: game

ifeq ($(wildcard Makefiles/$(COMPILEPATH)/Makef*), Makefiles/$(COMPILEPATH)/Makefile)
  include Makefiles/$(COMPILEPATH)/Makefile
else
  include Makefiles/Default/Makefile
endif

