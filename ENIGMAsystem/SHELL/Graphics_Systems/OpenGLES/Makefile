SOURCES += $(wildcard Graphics_Systems/OpenGLES/*.cpp) $(wildcard Graphics_Systems/General/*.cpp) Graphics_Systems/General/glew.c

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif

#$(OBJDIR)/Graphics_Systems/OpenGLES/glew.o: Graphics_Systems/OpenGLES/glew.c
#	gcc -c $^ -O3 -DGLEW_STATIC -o $@
