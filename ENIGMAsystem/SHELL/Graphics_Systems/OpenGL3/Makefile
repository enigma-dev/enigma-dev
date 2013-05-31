SOURCES += $(wildcard Graphics_Systems/OpenGL3/*.cpp) Graphics_Systems/OpenGL3/glew.c

ifeq ($(PLATFORM), Win32)
	LDLIBS += -lopengl32 -lglu32
else ifeq ($(PLATFORM), xlib)
	LDLIBS += -lGL -lGLU
else ifeq ($(PLATFORM), Cocoa)
	LDLIBS += -framework OpenGL
endif

#$(OBJDIR)/Graphics_Systems/OpenGL3/glew.o: Graphics_Systems/OpenGL3/glew.c
#	gcc -c $^ -O3 -DGLEW_STATIC -o $@

